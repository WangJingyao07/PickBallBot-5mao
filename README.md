# 🌈🔥 PickBallBot-5mao

<p align="left">
    <img alt="ViewCount" valign="bottom" src="https://views.whatilearened.today/views/github/WangJingyao07/PickBallBot-5mao.svg">
    <a href='https://github.com/WangJingyao07/github-clone-count-badge'><img alt='GitHub Clones' valign="bottom" src='https://img.shields.io/badge/dynamic/json?color=success&label=Clone&query=count&url=https://gist.githubusercontent.com/MShawon/cf89f3274d06170b8a4973039aa6220a/raw/clone.json&logo=github'></a>
    <img alt="releases" valign="bottom" src="https://img.shields.io/github/downloads/WangJingyao07/PickBallBot-5mao/total"> <code>since Sep 26</code>
</p>


🈶 This article is written in Chinese/English

❗ 仅用于个人学术研究

❗ For personal academic research only

❗ 涉及版权，因此只公布自己修正的可公开代码部分与结构

<div align="center">
	<img src="https://github.com/WangJingyao07/PickBallBot-5mao/assets/45681444/3a65b772-883a-44cd-ac88-9217ce060105" alt="Editor" width="500">
</div>

## Intro
- 🎉 一个低预算，添加了各种trick的高准确率自主捡球机器人，500块“无中生有”构建属于自己的机器人，走过路过不要错过，无奇怪bug不到一周即可实现。包括自动与手动两种控制方法，仓库提供包括结构建模，底层PID控制，视觉算法等所有需求文件。

- ✨ A low-budget, high-accuracy autonomous ball-picking robot with various tricks added, 500 yuan "made out of nothing" to build your own robot, don't miss it when you pass by, and it can be realized in less than a week without strange bugs. Including automatic and manual control methods, the warehouse provides all requirements documents including structural modeling, underlying PID control, and visual algorithms.

## Talk about this Bot
进行了一台机器人的组装与调试，主要包括机器人结构搭建与调整、机器人控制系统设计与实现、机器人感知系统的设计与实现及机器人软件编程等。这样的设置可以将组装一个机器人分为：结构设计，底层运动控制，视觉图像算法，串口通信，以及最后的整机调试与优化。

The assembly and debugging of a robot was carried out, mainly including the construction and adjustment of the robot structure, the design and implementation of the robot control system, the design and implementation of the robot perception system, and the robot software programming. Such a setting can divide the assembly of a robot into: structural design, low-level motion control, visual image algorithm, serial communication, and final machine debugging and optimization.

我们将主要从外观结构，以及算法驱动两个角度进行简单介绍

We will give a brief introduction mainly from the perspectives of appearance structure and algorithm drive

![image](https://github.com/WangJingyao07/PickBallBot-5mao/assets/45681444/7e14666f-4e5e-40fc-8703-0f29a916748e)

### 5 mao's photo

🛴 ori 5-mao
<div align="center">
	<img src="https://github.com/WangJingyao07/PickBallBot-5mao/assets/45681444/3a65b772-883a-44cd-ac88-9217ce060105" alt="Editor" width="500">
</div>

🚗 5-mao with great outfit
<div align="center">
	<img src="https://github.com/WangJingyao07/PickBallBot-5mao/assets/45681444/ceeebe44-9a9c-4933-ac78-432db8026802" alt="Editor" width="500">
</div>

### 结构设计

将整车分为主体、运动机构、乒乓球捡拾机构三个部分。

- 1️⃣ 主体结构：上位机为i.MX6开发板，下位机为STM32，传感器加装乐视摄像头，外壳利用光敏树脂3D打印，上位机与保护外壳之间利用铝合金板车成。外壳利用泡沫板、塑料、硬纸板进行了简单的改装
- 2️⃣ 运动机构：由底层加装的四个步进电机驱动的麦克纳姆轮组成
- 3️⃣ 捡拾部分：由两个摩擦轮与一对3D打印聚乙烯牛角状聚拢装置组成

### Structural Design

The whole vehicle is divided into three parts: the main body, the movement mechanism, and the table tennis ball picking mechanism.

- 1️⃣ Main structure: the upper computer is i.MX6 development board, the lower computer is STM32, the sensor is equipped with a LeTV camera, the outer shell is 3D printed with photosensitive resin, and the upper computer and the protective shell are made of aluminum alloy pallets. The shell is simply modified using foam board, plastic, cardboard
- 2️⃣ Motion mechanism: composed of four stepper motor-driven mecanum wheels installed on the bottom layer
- 3️⃣ Picking part: It consists of two friction wheels and a pair of 3D printed polyethylene horn-shaped gathering devices



### 算法&驱动

整车是通过三个节点进行驱动的，分别是图像检测节点，底层运动控制节点以及串口通信节点。

- 1️⃣ 图像检测节点：主要由上位机运算控制，主要通过边缘检测以及霍夫圆检测完成，同时，因为系统硬件性能较差的缘故，我们采用了手动视角偏移，以及基于patch的检测来校正位姿并降低所占算力，大大提高了捡拾效率。
- 2️⃣ 底层运动控制节点：包括简单逻辑控制与PID控制，主要由下位机进行控制，通过对四个麦轮进行控制，根据传入下位机的图像位置信息，驱动四个麦轮完成转向，运动步长设置，自旋转寻找目标等动作。
- 3️⃣ 串口通信节点：ROS通信，蓝牙通信

### Algorithms & Drivers

The whole vehicle is driven by three nodes, which are the image detection node, the underlying motion control node and the serial communication node.

- 1️⃣ Image detection node: It is mainly controlled by the operation of the host computer, mainly through edge detection and Hough circle detection. At the same time, because of the poor performance of the system hardware, we use manual angle of view offset and patch-based detection to correct pose and reduce the computing power, which greatly improves the picking efficiency.
- 2️⃣ Bottom motion control node: including simple logic control and PID control, mainly controlled by the lower computer. By controlling the four wheat wheels, the four wheat wheels are driven to complete the steering and movement according to the image position information transmitted to the lower computer. Step setting, self-rotation to find the target and other actions.
- 3️⃣ Serial communication nodes: ROS communication, Bluetooth communication

