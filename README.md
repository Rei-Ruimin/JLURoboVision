# Jilin University Team TARS-GO Vision code  JLURoboVision
---

## Thanks
First we'd like to thank Southeast University for its 2018 open source, Shenzhen University and SJTU for their 2019 open sources, they gave great contribution to this project and we hope this one could also help other teams to improve in RM competition.

---
## Introduction
This project is the infantry vision algorithm for Robomaster2020 competition of Team TARS-GO from Jilin University. There are five main modules: **Armor Detection**, **Power Rune Detection**, **Angle Calculation**, **Camera Driver** and **Serial Port/CAN Communication**.

---
## Directory
* [1. Function Introduction](#1Function-Introduction)
* [2. Show Results](#2Show-Results)
* [3. Dependencies](#3Dependencies)
* [4. Overall Framework](#4Overall-Framework)
* [5. Implementation Plan](#5Implementation-Plan)
* [6. Communication Protocol](#6Communication-Protocol)
* [7. Configuration and Debugging](#7Configuration-and-Debugging)
* [8. Summary and Outlook](#8Summary-and-Outlook)
---
## 1.Function Introduction
|Module    |Function     |
| ------- | ------ |
|Armor Detection| Detect the position of enemy robot armors and identify the numbers |
|Power Rune Detection|  Detect the position of inactived armor modules of Power Rune |
|Angle Calculation| Calculate yaw, pitch angles and the distance between target and barrel based on above position information |
|Camera Driver|  Use Daheng camera SDK package to implement camera parameters control and image acquisition |
|Serial Port/CAN Communication| Communicate with the lower computer(下位机), transmit the robot posture information and the operator feedback of visual control information |
---
## 2.Show Results
### Armor Detection
Armor detection adopts the traditional algorithm based on OpenCV to achieve the position detection of armor plates, it also adopts SVM to achieve the number detection of armor plates.
Considering the actual situation of the battlefield, the effective attack range of robots is between 1m and 7m. Within this range, **the armor detection rate of this algorithm is 98%**, and the coordinate information of the four vertices and center points of armor plates in the image can be obtained.
<div align=center>**EnemyColor = BLUE; TargetNum = 1**</div>  

<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/B1.png" width = "600" alt="图2.1 装甲板识别"/>
</div>  

 
<div align=center>**EnemyColor = RED; TargetNum = 2**</div>  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/R.png" width = "600" alt="图2.2 装甲板识别"/>
</div>  

With 640\*480 image resolution, **the armor detection frame rate is about 340fps, and can achive 420fps after the introduction of ROI**. However, considering the saturation of the recognition frame rate to the electronic control mechanical delay, the introduction of ROI operation is cancelled to avoid the problem that robots cannot detect global field of view in time, and to speed up the self-targeting process of robots.
<div align=center>**640\480（maximum 340FPS）**</div>  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/armor640480.gif" width = "600" alt="图2.3 装甲板实时识别帧率"/>
</div>  

<div align=center>**320*240（maximum 1400FPS）**</div>  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/armor320240.gif" width = "600" alt="图2.4 装甲板实时识别帧率"/>
</div> 

Armor number detection adopts SVM, crops the binarized armor image based on calculated armor position, applies transmission transformation and then uses trained SVM model to classify the armor number. **The accuracy rate of number detection can reach 98%**.
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/RealtimeArmor.gif" width = "600" alt="图2.5 装甲板数字识别"/>
</div> 

 
### Power Rune Detection
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/windmill.gif" width = "600" alt="图2.6 大风车识别演示"/>
</div> 
 
### Angle Calculation 
In terms of angle calculation, two calculation methods are used for diffrent distances. The first one uses the P4P algorithm, and the second one uses the PinHole algorithm based on small hole imaging.
In addition, we also introduced the camera-muzzle Y-axis distance compensation and gravity compensation.
Test with the calibration board, the distance error of angle calculation is within 10%, and the calculated angle is basically consistent with the real one.
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/pos.jpg" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle_solver.gif" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
 
---
## 3.Dependencies
### Hardware
|Hardware|Model|Parameter|
|---|---|---|
|Computing Platform|Manifold2-G|Tx2|
|Camera|Daheng Camera MER-050|Resolution 640*480, Exposure Value 3000~5000|
|Lens|M0814-MP2|Focal length 8mm, Aperture value 4|
### Software
|Software|Model|
|---|---|
|OS|Ubuntu 16.04/Ubuntu18.04|
|IDE|Qt Creator-4.5.2|
|Library|OpenCV-3.4.0|
|DRIVE|Galaxy SDK|
---
## 4.Overall Framework
### File Tree 
```
JLURoboVision/
├── AngleSolver
│   └── AngleSolver.h（header file of angle calculation module）
│   ├── AngleSolver.cpp（source file of angle calculation module）
├── Armor
│   ├── Armor.h（header file of armor detection module）
│   ├── LightBar.cpp（source file of light bar class ）
│   ├── ArmorBox.cpp（source file of armor plate class ）
│   ├── ArmorNumClassifier.cpp（source file of armor number classify class）
│   ├── findLights.cpp（source file of light bar finder related functions）
│   └── matchArmors.cpp（source file of armor source file matching related functions）
│   ├── ArmorDetector.cpp（source file of armor plate detection subclass）
├── General
│   ├── 123svm.xml（SVM module file）
│   ├── camera_params.xml（camera parameter file）
│   └── General.h（header file of general content declaration）
├── GxCamera
│   ├── GxCamera.h（header file of Daheng camera class）
│   ├── GxCamera.cpp（source file of packaging Daheng camera calss）
│   └── include（files included by camera SDK）
│       ├── DxImageProc.h
│       └── GxIAPI.h
└── Serial
│   ├──  Serial.h（header file of serial port）
│   └──Serial.cpp（source file of serial port）
├── Main
│   ├── ArmorDetecting.cpp（armor detection thread）
│   ├── ImageUpdating.cpp（image updating thread）
│   └── main.cpp（program main entrance）

```
### Overall Framework 
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/Armor.png " width = "800" alt="图4.1 自瞄算法流程图"/>
</div>  

---
## 5.Implementation Plan
### Armor Detection
装甲板识别使用基于检测目标特征的OpenCV传统方法，实现检测识别的中心思想是找出图像中所有敌方颜色灯条，并使用找出的灯条一一拟合并筛选装甲板。  
主要步骤分为：**图像预处理**、**灯条检测**、**装甲板匹配**、**装甲板数字识别**及最终的**目标装甲板选择**。  
Armor detection uses the traditional OpenCV method based on detecting target features. The main idea of detection is to find all light bars with enemy color in the image, and use these light bars to fit armor plates group by group.
The main steps are: image preprocessing, light bar detection, armor plate matching, armor plate number classif recognition and final target armor plate selection.

1. **图像预处理**  
为检测红/蓝灯条，需要进行颜色提取。颜色提取基本思路有BGR、HSV、通道相减法。  
然而，前两种方法由于需要遍历所有像素点，耗时较长，因此我们选择了**通道相减法**进行颜色提取。  
其原理是在**低曝光**（3000~5000）情况下，蓝色灯条区域的B通道值要远高于R通道值，使用B通道减去R通道再二值化，能提取出蓝色灯条区域，反之亦然。  
此外，我们还对颜色提取二值图进行一次掩膜大小3*3，形状MORPH_ELLIPSE的膨胀操作，用于图像降噪及灯条区域的闭合。  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/src_binary.jpg " width = "600" alt="图5.1 颜色提取二值图"/>
</div>  

2. **灯条检测**  
灯条检测主要是先对预处理后的二值图找轮廓（findContours），  
然后对初筛（面积）后的轮廓进行拟合椭圆（fitEllipse），  
使用得到的旋转矩形（RotatedRect）构造灯条实例（LightBar），  
在筛除偏移角过大的灯条后依据灯条中心从左往右排序。  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/Light_Monitor.jpg " width = "600" alt="图5.2 灯条识别图"/>
</div>  

3. **装甲板匹配**  
分析装甲板特征可知，装甲板由两个长度相等互相平行的侧面灯条构成，  
因此我们对检测到的灯条进行两两匹配，  
通过判断两个灯条之间的位置信息：角度差大小、错位角大小、灯条长度差比率和X,Y方向投影差比率，  
从而分辨该装甲板是否为合适的装甲板（isSuitableArmor），  
然后将所有判断为合适的装甲板放入预选装甲板数组向量中。  
同时，为了消除“游离灯条”导致的误装甲板，我们针对此种情况编写了eraseErrorRepeatArmor函数，专门用于检测并删除错误装甲板。  

```
/**
 *@brief: detect and delete error armor which is caused by the single lightBar 针对游离灯条导致的错误装甲板进行检测和删除
 */
void eraseErrorRepeatArmor(vector<ArmorBox> & armors)
{
	int length = armors.size();
	vector<ArmorBox>::iterator it = armors.begin();
	for (size_t i = 0; i < length; i++)
		for (size_t j = i + 1; j < length; j++)
		{
			if (armors[i].l_index == armors[j].l_index ||
				armors[i].l_index == armors[j].r_index ||
				armors[i].r_index == armors[j].l_index ||
				armors[i].r_index == armors[j].r_index)
			{
				armors[i].getDeviationAngle() > armors[j].getDeviationAngle() ? armors.erase(it + i) : armors.erase(it + j);
			}
		}
}
```
4. **装甲板数字识别**  
匹配好装甲板后，利用装甲板的顶点在原图的二值图（原图的灰度二值图）中剪切装甲板图，  
使用透射变换将装甲板图变换为SVM模型所需的Size，随后投入SVM识别装甲板数字。  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/NumClassifier.png " width = "600" alt="图5.3 装甲板数字识别图"/>
</div>  

5. **目标装甲板选取**  
对上述各项装甲板信息（顶点中心点坐标与枪口锚点距离、面积大小、装甲板数字及其是否与操作手设定匹配）进行加权求和，  
从而获取最佳打击装甲板作为最终的目标装甲板。  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/Armor_Monitor.png " width = "600" alt="图5.4 装甲板识别效果图"/>
</div>  

---
### 大风车识别  
首先对图像进行二值化操作，然后进行一定腐蚀和膨胀，通过边缘提取和条件限制得出待击打叶片（锤子形）。  
在待击打叶片范围内进一步用类似方法寻找目标装甲板和流动条，在二者连线上寻找中心的“R”。  
根据目标装甲板坐标和中心坐标计算极坐标系下的目标角度，进而预测待击打点的坐标（小符为装甲板本身，大符需要旋转）。  
最后将待击打点坐标和图像中心的差值转换为yaw和pitch轴角度，增加一环PID后发送给云台主控板。  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/windmill.png " width = "600" alt="图5.5 大风车识别"/>
</div>  

### 角度解算  
角度解算部分使用了两种模型解算枪管直指向目标装甲板所需旋转的yaw和pitch角。  
第一个是**P4P解算**，第二个是**PinHole解算**。  
首先回顾一下相机成像原理，其成像原理公式如下：  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle1.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
1. P4P解算原理  
由上述相机成像原理可得相机-物点的平移矩阵为：
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle2.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
转角计算公式如下：  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle3.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 

2. 小孔成像原理  
像素点与物理世界坐标系的关系：  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle4.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
则转角计算公式如下：  
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle5.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 

---
## 6.Communication Protocol
上下板之间的通信逻辑，主要由我们自定的通信协议体现：  
协议共有16个字节，包括帧头占用的1字节，校验位需要的1字节，数据位的12个字节，以及两个字节的标志位。可以满足上位机与主控板之间的通信需求，且尽量精简了数据包体量以提高传输速度。  
|Byte0|Byte1|Byte2|Byte3|Byte4|Byte5|Byte6|Byte7|
|:--|:--|:--|:--|:--|:--|:--|:--|
|0xAA|CRC_8|Yaw_data|Yaw_data|Yaw_data|Yaw_data|Pitch_data|Pitch_data|
|Byte8|Byte9|Byte10|Byte11|Byte12|Byte13|Byte14|Byte15|
|Pitch_data|Pitch_data|Dist_data|Dist_data|Dist_data|Dist_data|Targt)Num|Fire_cmd|
> * 0xAA -帧头 
> * Yaw_data : 8 bit char - 接收视觉解算出来的云台 yaw 值
> * Pitch_data : 8 bit char - 接收视觉解算出来的云台 pitch 值
> * （改为传360坐标系，x10保留一位小数，距离直接传数值，同样x10精度mm，帧头0XAA）
> * Dist_data : 8 bit char - 接收视觉解算目标到相机的距离值
> * Target_num : 8 bit char - 优先目标数字-前三位（数字0-8，没有6）（stm32 -> PC）
> * 模式选择 - 后五位（0 不处理，1-8留作模式选择， stm32 -> PC，1为自瞄，2为大风车）
> * Fire-cmd是否开火：8 bit char - 在视觉判定枪口（摄像头）对准目标在误差范围内时发送——0 为不开火，1 为开火
---
## 7.Configuration and Debugging
### 运行平台搭建  
1. Qt（及QtCreator）安装
2. OpenCV库安装及配置
3. 大恒相机驱动安装及配置

### 代码调试
1. 使用QtCreator打开JLURoboVision.pro（或直接在根目录中make）
2. 检查并修改camera_params.xml 及123svm.xml路径
3. 编译运行

### 单独模块调试  
可参考下列示例代码：  
[JLUVision_Demos](https://gitee.com/mountain123/JLUVision_Demos)各示例程序代码库  
[Armor_Demo](https://gitee.com/mountain123/JLUVision_Demos/tree/master/Armor_Demo)为装甲板识别模块演示程序，可在Linux(.pro)/Windows(.sln)运行。  
[AngleSolver_Armor_GxCamera](https://gitee.com/mountain123/JLUVision_Demos/tree/master/Anglesolver_Armor_GxCamera_Demo)为大恒相机采图+装甲板+角度解算演示程序，需要连接大恒相机在Linux下运行。  

### Debugging Tools  
代码还自定义了一套调试用的函数，将灯条、装甲板识别、角度解算等信息进行可视化输出，并可通过键盘控制部分识别参数，为代码的调试和优化带来便利。  
```
//装甲板检测识别调试参数是否输出
//param:
//		1.showSrcImg_ON,		  是否展示原图
//		2.bool showSrcBinary_ON,  是否展示二值图
//		3.bool showLights_ON,	  是否展示灯条图
//		4.bool showArmors_ON,	  是否展示装甲板图
//		5.bool textLights_ON,	  是否输出灯条信息
//		6.bool textArmors_ON,	  是否输出装甲板信息
//		7.bool textScores_ON	  是否输出打击度信息
//					   1  2  3  4  5  6  7
detector.showDebugInfo(0, 0, 0, 1, 0, 0, 0);
```

```
//角度解算调试参数是否输出
//param:
//		1.showCurrentResult,	  是否展示当前解算结果
//		2.bool showTVec,          是否展示目标坐标
//		3.bool showP4P,           是否展示P4P算法计算结果
//		4.bool showPinHole,       是否展示PinHole算法计算结果
//		5.bool showCompensation,  是否输出补偿结果
//		6.bool showCameraParams	  是否输出相机参数
//					      1  2  3  4  5  6
angleSolver.showDebugInfo(1, 1, 1, 1, 1, 0);
```
---
## 8.Summary and Outlook
### 总结  
本套代码主要实现了装甲板识别及大风车的识别这两个模块，结合角度解算模块对识别到的目标信息的解算，获取云台枪口控制转角，随后通过串口传输给下位机。  
装甲板识别与大风车识别模块性能表现不错，识别率和帧率满足比赛需求；角度解算模块经过设计，提升了准确性及鲁棒性。    
同时，代码整体经过封装，具有较强的可移植性。  
### 特色功能  
1. 丰富的调试接口及数据可视化  
代码配备了多个调试用函数，能将代码运行效果及计算参数通过图片或终端实时显示，便于代码调试优化。  
2. 深入底层的图像处理  
在预处理阶段，选用了通道相减进行颜色提取，然而通道相减法需要调用split及thresh等函数，耗时较长，于是我们经过分析算法特点，我们直接通过指针来遍历图像数据，大大加快了该步的运算速度。  
```
//pointer visits all the data of srcImg, the same to bgr channel split 通道相减法的自定义形式，利用指针访问，免去了split、substract和thresh操作，加速了1.7倍
//data of Mat  bgr bgr bgr bgr
uchar *pdata = (uchar*)srcImg.data;
uchar *qdata = (uchar*)srcImg_binary.data;
int srcData = srcImg.rows * srcImg.cols;
if (enemyColor == RED)
{
	for (int i = 0; i < srcData; i++)
	{
		if (*(pdata + 2) - *pdata > armorParam.color_threshold)
			*qdata = 255;
		pdata += 3;
		qdata++;
	}
}
else if (enemyColor == BLUE)
{
	for (int i = 0; i < srcData; i++)
	{
		if (*pdata - *(pdata+2) > armorParam.color_threshold)
			*qdata = 255;
		pdata += 3;
		qdata++;
	}
}
```
3. 目标装甲板加权计分选取  
目标装甲板的选取，我们结合了操作手指定兵种及装甲板实际打击特征（距离枪口的平移向量、打击面积大小）进行加权求和，最终选取打击度得分最大的作为目标装甲板。  
```
/**
 *@brief: compare a_armor to b_armor according to their distance to lastArmor(if exit, not a default armor) and their area and armorNum
 *		  比较a_armor装甲板与b_armor装甲板的打击度，判断a_armor是否比b_armor更适合打击（通过装甲板数字是否与目标装甲板数字匹配，装甲板与lastArmor的距离以及装甲板的面积大小判断）
 */
bool armorCompare(const ArmorBox & a_armor, const ArmorBox & b_armor, const ArmorBox & lastArmor, const int & targetNum)
{
	float a_score = 0;  // shooting value of a_armor a_armor的打击度
	float b_score = 0;  //shooting value of b_armor b_armor的打击度
	a_score += a_armor.armorRect.area(); //area value of a a_armor面积得分
	b_score += b_armor.armorRect.area(); //area value of b b_armor面积得分

	//number(robot type) priorty 设置a、b装甲板的分数
	setNumScore(a_armor.armorNum, targetNum, a_score);
	setNumScore(b_armor.armorNum, targetNum, b_score);
	
	if (lastArmor.armorNum != 0) {  //if lastArmor.armorRect is not a default armor means there is a true targetArmor in the last frame 上一帧图像中存在目标装甲板
		float a_distance = getPointsDistance(a_armor.center, lastArmor.center); //distance score to the lastArmor(if exist) 装甲板距离得分，算负分
		float b_distance = getPointsDistance(b_armor.center, lastArmor.center); //distance score to the lastArmor(if exist) 装甲板距离得分，算负分
		a_score -= a_distance * 2;
		b_score -= b_distance * 2;
	}
	return a_score > b_score; //judge whether a is more valuable according their score 根据打击度判断a是否比b更适合打击
}
```  
4. 角度解算具有两个计算模型分档运行  

### 展望  
1. 卡尔曼滤波预测
2. 深度学习识别
3. 计算平台性能提升
