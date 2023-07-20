#include<ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include "image/ping.h"  //自定义消息类型

#include <stdint.h>

using namespace std;

cv::Mat image0; //原始图像
cv::Mat image1;
cv::Mat image2;
cv::Mat image_canny;
int Thresh = 100;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "image_process");
	ros::NodeHandle n; 
	ros::Rate loop_rate(10);

	// 定义节点句柄   
	ros::Publisher ping_info_pub = n.advertise<image::ping>("/ping_info", 1);
	image::ping msg;

	cv::VideoCapture cap(2);
    cap.set(cv::CAP_PROP_FRAME_WIDTH,640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,480);
	if (!cap.isOpened())
	{
		ROS_INFO("Cannot open camera");
		return -1;
	}
	while(ros::ok())  //读取图像成功
	{
		ros::spinOnce();
		cap.read(image0);
	    cv::cvtColor(image0, image1, cv::COLOR_BGR2GRAY);
	    cv::medianBlur(image1,image2, 5);
	    //cv::blur(image2, image3, cv::Size(3, 3), cv::Point(-1, -1));
	    vector<cv::Vec3f> circles;
		vector<cv::Vec3f> circles_temp(1,0); //声明一个向量，初始化为0
		cv::Canny(image2, image_canny, Thresh, Thresh * 2, 3);
		cv::HoughCircles(image_canny, circles, CV_HOUGH_GRADIENT, 1, 30, 60, 25, 15, 300);
		if(circles.size() != 0) //检测到圆了
		{
			for (size_t i = 0; i < circles.size(); i++)
			{
				if(circles[i][2]>circles_temp[i][2])  //找出最大半径
					circles_temp[0] = circles[i];
			}
			#if 1   //条件编译    修改为0可取消图像显示
			cv::Point center(cvRound(circles_temp[0][0]), cvRound(circles_temp[0][1]));
			int radius = cvRound(circles_temp[0][2]);
			cv::circle(image0, center, radius, cv::Scalar(0, 255, 0), 5, 8, 0);
			cv::imshow("image",image0);
			cv::waitKey(1);
			ROS_INFO("publisher:    X:%d,Y:%d,R:%d",cvRound(circles_temp[0][0]),cvRound(circles_temp[0][1]),cvRound(circles_temp[0][2]));
			#endif
			msg.x = cvRound(circles_temp[0][0]);
			msg.y = cvRound(circles_temp[0][1]);
			msg.r = cvRound(circles_temp[0][2]);
			ping_info_pub.publish(msg);
		}
		else
		{
			msg.x =0;
			msg.y = 0;
			msg.r = 0;
			ping_info_pub.publish(msg);
			ROS_INFO("    none    ");
		}
		// cv::waitKey(700);
		loop_rate.sleep();

		
	}
	return 0;
}
