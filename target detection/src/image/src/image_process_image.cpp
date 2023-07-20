#include<ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include "image/ping.h"  //自定义消息类型
#include <stdint.h>

using namespace std;
const string file_path = "/home/jjl/catkin_ws/src/image_process/src/1.jpg"; //图片的绝对地址
cv::Mat image0; //原始图像
cv::Mat image1;
cv::Mat image2;
cv::Mat image_canny;
int Thresh = 100;
int main(int argc, char** argv)
{
	ros::init(argc, argv, "image_process_image");
	ros::NodeHandle n; 
	ros::Rate loop_rate(60);
	// 定义节点句柄   
	ros::Publisher ping_info_pub = n.advertise<image::ping>("/ping_info", 10);
	image::ping msg;
    while(1)
    {
		image0 = cv::imread(file_path);
		if(image0.empty())

		{
			ROS_INFO( "read image error");;
			return -1;
		}
		cv::cvtColor(image0, image1, cv::COLOR_BGR2GRAY);
		cv::medianBlur(image1,image2, 5);
		//cv::blur(image2, image3, cv::Size(3, 3),cv::Point(-1, -1));
		vector<cv::Vec3f> circles;           //声明一个向量，保存检测出的圆的圆心坐标和半径
		vector<cv::Vec3f> circles_temp(1,0); //声明一个向量，初始化为0
		cv::Canny(image2, image_canny, Thresh, Thresh * 2, 3);
		cv::HoughCircles(image_canny, circles, CV_HOUGH_GRADIENT, 1, 60, 150, 30, 0, 0);
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
			cv::waitKey(200);
			ROS_INFO("publisher:    X:%d,Y:%d,R:%d",cvRound(circles_temp[0][0]),cvRound(circles_temp[0][1]),cvRound(circles_temp[0][2]));
			#endif
			msg.x = cvRound(circles_temp[0][0]);
			msg.y = cvRound(circles_temp[0][1]);
			msg.r = cvRound(circles_temp[0][2]);
			ping_info_pub.publish(msg);
		}
		else
		{
			ROS_INFO("    none    ");
		}
		loop_rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
