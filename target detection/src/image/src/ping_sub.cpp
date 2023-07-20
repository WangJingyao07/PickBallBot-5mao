#include<ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include "image/ping.h"  //自定义消息类型
#include <stdint.h>


void pingInfoCallback(const image::ping::ConstPtr& msg)
{
    // 将接收到的消息打印出来
    ROS_INFO("subscriber:    X:%d Y:%d R:%d", 
			 msg->x, msg->y, msg->r);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ping_sub");
    ros::NodeHandle n;
    ros::Subscriber ping_info_sub = n.subscribe("/ping_info", 10, pingInfoCallback);
    ros::spin();

    return 0;
}
