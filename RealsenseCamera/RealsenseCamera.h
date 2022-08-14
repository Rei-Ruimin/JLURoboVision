#ifndef CAMERA_H
#define CAMERA_H

// Import librealsense header
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
// Let's define our camera as a thread, it will be constantly running and sending frames to
// our main window
class Camera
{
public:
    // We need to instantiate a camera with both depth and rgb resolution (as well as fps)
    Camera(int rgb_width, int rgb_height, int depth_width, int depth_height, int fps);
    ~Camera() {}

    // Member function that handles thread iteration
    void run(cv::Mat* targetMatImg);

    // If called it will stop the thread
    void stop() { camera_running = false; }

private:
    // Realsense configuration structure, it will define streams that need to be opened
    rs2::config cfg;

    // Our pipeline, main object used by realsense to handle streams
    rs2::pipeline pipe;

    // Frames returned by our pipeline, they will be packed in this structure
    rs2::frameset frames;

    // A bool that defines if our thread is running
    bool camera_running = true;

};

#endif // CAMERA_H

