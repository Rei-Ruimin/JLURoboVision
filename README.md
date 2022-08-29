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

5. **Target armor plate selection**
Perform weighted summation of the above-mentioned armor plate information (the distance between the center point of the vertex and the muzzle anchor point, the area size, the number of the armor plate and whether it matches the operator setting), then calculate the best armor plate as the final target.
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/Armor_Monitor.png " width = "600" alt="图5.4 装甲板识别效果图"/>
</div>  

---
### Power Rune Detection
First, the image is binarized, and then a certain amount of corrosion and expansion is applied, and the blade to be hit (hammer shape) is obtained through edge extraction and conditional restrictions.
In the area of the blade to be hit, a similar method is used to find the target armor plate and the flow bar, and the "R" could be found on the center of the line between these two.
Calculate the target angle in the polar coordinate system according to the coordinates of the target armor plate and the center coordinates, and then predict the coordinates of the point to be hit (for the small rune it is the armor plate itself, for the large rune we need to consider the rotation).
Finally, the difference between the coordinates of the point to be hit and the center of the image is converted into yaw and pitch axis angles, and a loop of PID is added and sent to the gimbal main control board.
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/windmill.png " width = "600" alt="图5.5 大风车识别"/>
</div>  

### Angle calculation
The angle calculation part uses two models to calculate the yaw and pitch angles for the muzzle to point directly at the target armor plate.
The first is **P4P solution**, and the second is **PinHole solution**.
First review the camera imaging principle, the principle formula is as follows:
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle1.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
1. Principle of P4P
According to the above-mentioned camera imaging principle, the translation matrix of camera-object point can be obtained as:
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle2.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
The formula for calculating the angle of rotation is as follows:
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle3.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 

2. Principle of pinhole camera
The relationship between the pixel and the physical world coordinate system:
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle4.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 
The formula for calculating the angle of rotation is as follows:
<div align=center>
<img src="https://github.com/QunShanHe/JLURoboVision/blob/master/Assets/angle5.png" width = "600" alt="图2.7 角度解算测试图"/>
</div> 

---
## 6.Communication Protocol
The communication logic between the upper and lower boards is mainly embodied by our custom communication protocol:
The protocol has a total of 16 bytes, including 1 byte occupied by the frame header, 1 byte required for the check bits, 12 bytes of data bits, and two bytes of flag bits. It can meet the communication requirements between the host computer and the main control board, and minimize the size of the data packet to increase the transmission speed.
|Byte0|Byte1|Byte2|Byte3|Byte4|Byte5|Byte6|Byte7|
|:--|:--|:--|:--|:--|:--|:--|:--|
|0xAA|CRC_8|Yaw_data|Yaw_data|Yaw_data|Yaw_data|Pitch_data|Pitch_data|
|Byte8|Byte9|Byte10|Byte11|Byte12|Byte13|Byte14|Byte15|
|Pitch_data|Pitch_data|Dist_data|Dist_data|Dist_data|Dist_data|Targt)Num|Fire_cmd|
> * 0xAA-frame header
> * Yaw_data: 8 bit char - Receive the calculated PTZ yaw value
> * Pitch_data: 8 bit char - Receive the calculated PTZ pitch value
> * (Change to the 360 coordinate system, x10 retains one decimal place; the distance is directly transferred as number value, use the same x10 with mm precision, frame header 0XAA)
> * Dist_data: 8 bit char - Receive the calculated distance between the target and camera
> * Target_num: 8 bit char - priority target number - the first three digits (numbers 0-8, without 6) (stm32 -> PC)
> * Mode selection - the last five digits (0 is not processed, 1-8 are reserved for mode selection, stm32 -> PC, 1 is for auto-shooting, 2 is for Power Rune)
> * Fire-cmd (fires or not): 8 bit char - sent when the  vision algorithm judges that the muzzle (camera) aims at the target within the error range - 1 means fire, 0 means not
---
## 7.Configuration and Debugging
### Platform Construction
1. Qt (& QtCreator) Installation
2. OpenCV Library Installation & Deployment
3. DaHeng Camera Driver Installation & Deployment

### Code Debugging
1. Use QtCreator to open JLURoboVision.pro (or 'make' directly in root directory)
2. Check & Modify path/to/camera_params.xml and path/to/123svm.xml
3. Compile & Run

### Unit-Module Debugging
the following codes are for reference：  
[JLUVision_Demos](https://gitee.com/qunshanhe/JLUVision_Demos) - sample program libraries\
[Armor_Demo](https://gitee.com/qunshanhe/JLUVision_Demos/tree/master/Armor_Demo) - demo of armor board detection module, can run on Linux(.pro)/Windows(.sln).\
[AngleSolver_Armor_GxCamera](https://gitee.com/qunshanhe/JLUVision_Demos/tree/master/Anglesolver_Armor_GxCamera_Demo) - demo of DaHeng camera's image capture + armor board + angle solver program, need to connect DaHeng camera and run in Linux

### Debugging Tools  
The code also customizes a set of debugging functions to visualize output information such as light bar, armor plate recognition, angle calculation, etc., and the recognition parameters can be controlled through the keyboard, which brings convenience to the debugging and optimization of the code.
```
//Whether to show armor board detection and identification debugging parameters


//param:
//		1.showSrcImg_ON,		  whether to show the original picture
//		2.bool showSrcBinary_ON,  whether to show the binary graph
//		3.bool showLights_ON,	  whether to show the light bar graph
//		4.bool showArmors_ON,	  whether to show the armor board graph
//		5.bool textLights_ON,	  whether to output the light bar information
//		6.bool textArmors_ON,	  whether to output the armor board information
//		7.bool textScores_ON	  whether to output the attacking score
//					   1  2  3  4  5  6  7
detector.showDebugInfo(0, 0, 0, 1, 0, 0, 0);
```

```
//WWhether to show the angle calculation debugging parameters
//param:
//		1.showCurrentResult,	  whether to show the current calculation result
//		2.bool showTVec,          whether to show the target coordinates
//		3.bool showP4P,           whether to show the result of P4P algorithm
//		4.bool showPinHole,       whether to show the result of PinHole algorithm
//		5.bool showCompensation,  whether to output the compensation result
//		6.bool showCameraParams	  whether to output camera parameters
//					      1  2  3  4  5  6
angleSolver.showDebugInfo(1, 1, 1, 1, 1, 0);
```
---
## 8.Summary and Outlooks
### Summary
This set of codes mainly implements the two modules of armor board detection and power rune detection, combined with the information of detected target by the angle calculation module, obtains the control angle of the gimbal muzzle, and then transmits it to the lower computer through the serial port.
The performance of the armor board detection and  power rune detection modules is good, and the recognition rate and frame rate meet the requirements of the competition; the angle calculation module has been designed to improve the accuracy and robustness.
At the same time, the code as a whole is encapsulated and has strong portability.

### Special Functionalities
1. Rich debugging interfaces and data visualization
2. The code is equipped with multiple debugging functions, which can display the code running results and calculation parameters through pictures or terminals in real time, which is convenient for code debugging and optimization.
3. Deep into the underlying image processing
In the preprocessing stage, channel subtraction is used for color extraction. However, the channel subtraction method needs to call functions such as split and thresh, which takes a long time. Therefore, after analyzing the characteristics of the algorithm, we directly traverse the image data through the pointer, which greatly speeds up the calculation speed of this step.
```
//A custom form of channel subtraction, using pointer access, eliminating split, substract and thresh functions, speeding up by 1.7 times
//pointer visits all the data of srcImg, the same as bgr channel split
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
4. Selection of target armor board with weighted scores
To select the target armor board, we combined the operator's designated robots and the actual attack-related characteristics of the armor board (The translation vector from the muzzle and the size of the attacking area, ect) for weighted summation, and finally selected the target armor board with the highest attacking score. 
```
/**
 *@brief: compare a_armor to b_armor based on their distance to lastArmor(if exit, not a default armor), size of the attacking area and operator's designated robots (armorNum)
 */
bool armorCompare(const ArmorBox & a_armor, const ArmorBox & b_armor, const ArmorBox & lastArmor, const int & targetNum)
{
	float a_score = 0;  // attacking score of a_armor 
	float b_score = 0;  //attacking score of b_armor 
	a_score += a_armor.armorRect.area(); //area socre of a
	b_score += b_armor.armorRect.area(); //area socre of b

	//number(robot type) priorty based on operator's designated robots
	setNumScore(a_armor.armorNum, targetNum, a_score);
	setNumScore(b_armor.armorNum, targetNum, b_score);
	
	if (lastArmor.armorNum != 0) {  //if lastArmor.armorRect is not a default armor means there is a true targetArmor in the last frame
		float a_distance = getPointsDistance(a_armor.center, lastArmor.center); //distance score to the lastArmor(if exist)
		float b_distance = getPointsDistance(b_armor.center, lastArmor.center); //distance score to the lastArmor(if exist)
		a_score -= a_distance * 2;
		b_score -= b_distance * 2;
	}
	return a_score > b_score; //judge whether a is more valuable according their score 
}
```  
5. Angle solver has two calculation models to run in stages

### Outlook
1. Kalman filter prediction
2. Deep learning identification
3. Computing platform performance improvement
