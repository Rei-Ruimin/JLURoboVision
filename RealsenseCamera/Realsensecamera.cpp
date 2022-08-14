
#include"RealsenseCamera.h"

extern pthread_mutex_t Globalmutex; // threads conflict due to image-updating
extern pthread_cond_t GlobalCondCV; // threads conflict due to image-updating
extern bool imageReadable;          // threads conflict due to image-updating
//extern cv::Mat src;                     // Transfering buffer

using namespace std;
using namespace cv;

Camera::Camera(int rgb_width, int rgb_height, int depth_width, int depth_height, int fps)
{
    // Enable depth stream with given resolution. Pixel will have a bit depth of 16 bit
//    cfg.enable_stream(RS2_STREAM_DEPTH, depth_width, depth_height, RS2_FORMAT_Z16, fps);

    // Enable RGB stream as frames with 3 channel of 8 bit
//    cfg.enable_stream(RS2_STREAM_COLOR, rgb_width, rgb_height, RS2_FORMAT_RGB8, fps);
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480,RS2_FORMAT_BGR8);

    // Start our pipeline
    rs2::pipeline_profile selection = pipe.start(cfg);
//    rs2::device selected_device = selection.get_device();
//    auto depth_sensor = selected_device.first<rs2::depth_sensor>();
//   depth_sensor.get_option()

    auto rgbSensor = selection.get_device().query_sensors()[1];
    rgbSensor.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0);
    rgbSensor.set_option(RS2_OPTION_EXPOSURE, 5);
    rgbSensor.set_option(RS2_OPTION_GAIN, 10);
    rs2::region_of_interest roi;
    rs2::roi_sensor s(rgbSensor);
        roi.min_x = 0;
        roi.max_x = 640;
        roi.min_y = 0;
        roi.max_y = 480;
    std::vector<rs2::sensor> sensors_started = pipe.get_active_profile().get_device().query_sensors();
//    roi = s.get_region_of_interest();
    for (rs2::sensor sensor_in : sensors_started){
        rs2::roi_sensor sensor(sensor_in);

        try {
            sensor.set_region_of_interest(roi);
        } catch (const rs2::invalid_value_error& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

void Camera::run(cv::Mat* targetMatImg)
{
    while(camera_running)
    {
        // Wait for frames and get them as soon as they are ready
        frames = pipe.wait_for_frames();

        // Let's get our depth frame
//        rs2::depth_frame depth = frames.get_depth_frame();setRoiParam
        // And our rgb frame
        rs2::frame rgb = frames.get_color_frame();

        using namespace rs2;

        auto vf = rgb.as<video_frame>();
        const int w = vf.get_width();
        const int h = vf.get_height();

//        cv::Mat src;
//        src.create(h,w,CV_8UC3);
        cv::Mat src = Mat(Size(w, h), CV_8UC3, (void*)rgb.get_data(), Mat::AUTO_STEP);
//        uchar* pBGRBuf = NULL;
//        pBGRBuf = new uchar[h*w*3];
//        pBGRBuf = rgb.get_data();

//        memcpy(src.data,pBGRBuf,h*w*3);

        // producer 获取Mat图像接口
        pthread_mutex_lock(&Globalmutex);
//                src.copyTo(*threadParam->m_pImage);
        src.copyTo(*targetMatImg);
        imageReadable = true;
        pthread_cond_signal(&GlobalCondCV);
        pthread_mutex_unlock(&Globalmutex);

//        delete[] pBGRBuf;
//        pBGRBuf = NULL;
    }
}
