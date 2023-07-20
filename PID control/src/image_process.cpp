#include <ros/ros.h>   
#include <image_transport/image_transport.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/point_cloud.h>


//******include PCL相关库文件***********
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl_conversions/pcl_conversions.h>

#include <string>
#include <sstream>
#include <math.h>

#define SHOW_IMAGE
//**********相机内参**************
const double camera_factor = 1000;
const double camera_cx = 321.798;
const double camera_cy = 239.607;
const double camera_fx = 615.899;
const double camera_fy = 616.468;

double distance_0 = 0;

using namespace std;
bool rgb_ok = false,dep_ok = false;

//*******变量*********
cv_bridge::CvImagePtr rgb_ptr,dep_ptr;
cv::Mat rgb_pic, dep_pic;
cv::Mat img_hsv; //原始图像
cv::Mat img_blu;
cv::Mat img_mask;
cv::Mat img_canny;

int Thresh = 60;

int iLowH = 3;
int iHighH = 25;
int iLowS =109;
int iHighS = 255;
int iLowV = 160;
int iHighV = 255;


void rgb_Callback(const sensor_msgs::ImageConstPtr& rgb_msg)
{
  try
  {
    #ifdef SHOW_IMAGE
      // cv::imshow("rgb_view", cv_bridge::toCvShare(rgb_msg, sensor_msgs::image_encodings::BGR8)->image);
      // cv::waitKey(1);
    #endif
    rgb_ptr = cv_bridge::toCvCopy(rgb_msg, sensor_msgs::image_encodings::BGR8); 
    rgb_pic = rgb_ptr->image;
    rgb_ok = true;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'mono16'.", rgb_msg->encoding.c_str());
  }
}

//*****************订阅深度图像处理回调函数*****************
void dep_Callback(const sensor_msgs::ImageConstPtr& dep_msg)
{
  try
  {
    #ifdef SHOW_IMAGE
      // cv::imshow("dep_view", cv_bridge::toCvShare(dep_msg, sensor_msgs::image_encodings::TYPE_32FC1)->image);
      // cv::waitKey(1);
    #endif
    dep_ptr = cv_bridge::toCvCopy(dep_msg, sensor_msgs::image_encodings::TYPE_32FC1); 
    dep_pic = dep_ptr->image;
    dep_ok = true;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'mono16'.", dep_msg->encoding.c_str());
  }
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_process");
  ros::NodeHandle n;
  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub_rgb = it.subscribe("/camera/rgb/image_raw", 1, rgb_Callback);
  image_transport::Subscriber sub_dep = it.subscribe("/camera/depth/image", 1, dep_Callback);
  // ros::Publisher pointcloud_publisher = nh.advertise<sensor_msgs::PointCloud2>("generated_pc", 1);
  ros::Rate naptime(5); //频率设置
  cout<<"****** ros set ok ******: "<<endl;
  while (ros::ok()) 
  {
    if(rgb_ok && dep_ok)//图像准备完毕
    {
      cv::GaussianBlur(rgb_pic,img_blu, cv::Size(5,5),0);
      vector<cv::Mat> hsvSplit;
      cv::cvtColor(img_blu, img_hsv, cv::COLOR_BGR2HSV);
      cv::split(img_hsv, hsvSplit);
      cv::equalizeHist(hsvSplit[2], hsvSplit[2]);
      cv::merge(hsvSplit, img_hsv);
      cv::inRange(img_hsv, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), img_mask);
	    vector<cv::Vec3f> circles;
      vector<cv::Vec3f> circles_temp(1,0); //声明一个向量，初始化为0
      cv::Canny(img_mask, img_canny, Thresh, Thresh * 2, 3);
      cv::HoughCircles(img_canny, circles, CV_HOUGH_GRADIENT, 1, 25, 60, 20, 5, 150);
      if(circles.size() != 0) //检测到圆了
      {
        for (size_t i = 0; i < circles.size(); i++)
        {
          if(circles[i][2]>circles_temp[i][2])  //找出最大半径
            circles_temp[0] = circles[i];
        }
        //    开始测量距离
        int m=circles_temp[0][0],n=circles_temp[0][1];
        float d = dep_pic.ptr<float>(m)[n];
	printf("d is %f",d);
        if (isnan(d))// d 可能没有值，若如此，跳过此点
          {
            distance_0 = 0;
            // break;
          }
        else // d 存在值，则向点云增加一个点
          {
            float x=0,y=0,z=0;
            // 计算这个点的空间坐标
            z = double(d) / camera_factor;
            x = (n - camera_cx) * z / camera_fx;
            y = (m - camera_cy) * z / camera_fy;
            distance_0 = sqrt(pow(x,2) + pow(y,2)+ pow(z,2))*100000;//转化为cm单位
          }  
        ROS_INFO(" *************distance=%f***********  ",distance_0);
        
      }
      else
      {
        distance_0 = 0;
        ROS_INFO("    none    ");
      }
      #ifdef SHOW_IMAGE
        cv::Point center(cvRound(circles_temp[0][0]), cvRound(circles_temp[0][1]));
			  int radius = cvRound(circles_temp[0][2]);
			  cv::circle(rgb_pic, center, radius, cv::Scalar(0, 255, 0), 5, 8, 0);
        // string res;
        // stringstream ss;
        // ss << distance_0;
        // ss >> res;  
        // cv::putText(rgb_pic, res, center, cv::FONT_HERSHEY_TRIPLEX, 0.8, cv::Scalar(0, 0, 255), 2, CV_AA);
        cv::imshow("img_mask", img_mask);
        cv::imshow("output", rgb_pic);
        cv::waitKey(1);
      #endif
      rgb_ok = false,dep_ok = false;
    }
    ros::spinOnce(); //allow data update from callback; 
    naptime.sleep(); // wait for remainder of specified period; 
  }
}
