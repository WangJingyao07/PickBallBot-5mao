#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <image_process/ping.h>
#include <iostream>
using namespace std;

float P = 0.003, I = 0.05, D = 0.02;
float err0 = 0, err1 = 0, err2 = 0;
float PID_Out = 0;
const int Ping_Center = 320;
geometry_msgs::Twist cmd_vel_msg;


void Ping_Callback(const image_process::ping::ConstPtr& msg);

void Ping_Callback(const image_process::ping::ConstPtr& msg)
{
  ROS_INFO("Subcribe Ping Info: x:%d  y:%d  r:%d",\
  msg->x, msg->y, msg->r);
  err0 = Ping_Center - (float)msg->x;
  if(msg->x > 0){  //有球才计算pid
	  PID_Out = P * err0 + D * (err1 - err2);
  }
  else{
	  PID_Out = 0;
  }
//   error record;
  err2 = err1;
  err1 = err0;
  if(PID_Out == 0){  //看不到东西的时候旋转
	  cmd_vel_msg.linear.x = 0;
	  cmd_vel_msg.linear.y = 0;
	  cmd_vel_msg.linear.z = 0;
	  cmd_vel_msg.angular.x = 0;
	  cmd_vel_msg.angular.y = 0;
	  cmd_vel_msg.angular.z = 0.3;
  }
  else{ //看到球了
	  cmd_vel_msg.linear.x = 0.2;  //固定速度前进
	  cmd_vel_msg.linear.y = 0;
	  cmd_vel_msg.linear.z = 0;
	  cmd_vel_msg.angular.x = 0;
	  cmd_vel_msg.angular.y = 0;
	  cmd_vel_msg.angular.z = PID_Out;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ping_subs");
  ros::NodeHandle n;
  ros::Subscriber Ping_sub = n.subscribe("/ping_info", 1, Ping_Callback);
  ros::Publisher Cmd_vel = n.advertise<geometry_msgs::Twist>("cmd_vel",1);
  ros::Rate loop_rate(60);
  cout<<"****** ros set ok ******: "<<endl;
  while (ros::ok()) 
  {
    ros::spinOnce();
    Cmd_vel.publish(cmd_vel_msg);
  }
}

