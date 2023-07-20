#! /usr/bin/env python
# encoding: utf-8
import rospy
from image.msg import ping
from geometry_msgs.msg import Twist
import time


class ping_sub():
    def __init__(self):

        # self.rate = rospy.Rate(0.5)



        self.revolve_time = 0
        self.dt = 0.0026  # 控制小车微调单位时间
        self.lb = 100  # 捡球左边界
        self.rb = 600  # 捡球右边界
        self.move_stop = [0, 0, 0, 0, 0, 0]
        self.move_left = [0, 1.5, 0, 0, 0, 0]
        self.move_right = [0, -1.5, 0, 0, 0, 0]
        self.move_forward = [1.5, 0, 0, 0, 0, 0]
        self.move_cricle = [0, 0, 0, 0, 0, 1.5]

        # self.cmd_vel_pub = rospy.Publisher('cmd_vel', Twist, queue_size=5)
        self.twist = Twist()

        # rospy.loginfo("Waiting for ping_info topic...")
        # rospy.wait_for_message('ping_info', ping)

        self.image_sub = rospy.Subscriber("/ping_info", ping, self.Callback, queue_size=1)
        rospy.loginfo("Marker messages detected. Starting follower...")
        self.image_pub = rospy.Publisher('cmd_vel', Twist, queue_size=1)

    # while not rospy.is_shutdown():
    #     # Send the Twist command to the robot
    #     self.cmd_vel_pub.publish(self.move_cmd)

    #     # Sleep for 1/self.rate seconds
    #     r.sleep()
    def move(self, data):
        self.twist.linear.x = data[0];
        self.twist.linear.y = data[1];
        self.twist.linear.z = data[2]
        self.twist.angular.x = data[3];
        self.twist.angular.y = data[4];
        self.twist.angular.z = data[5]
        rospy.loginfo("data:    X:%d Y:%d Z:%dF:%d", self.twist.linear.x, self.twist.linear.y, self.twist.linear.z, self.twist.angular.z)
        self.image_pub.publish(self.twist)
        # self.rate.sleep()
        # self.twist.linear.x = 0
        # self.twist.linear.y = 0
        # self.twist.linear.z = 0
        # self.twist.angular.x = 0
        # self.twist.angular.y = 0
        # self.twist.angular.z = 0
        # self.image_pub.publish(self.twist)

    def Callback(self, msg):
        # rospy.loginfo("subscriber:    X:%d Y:%d R:%d", msg.x, msg.y, msg.r)
        car_move = [msg.x, msg.y, msg.r]

        # while (self.revolve_time <= 29.4):  # 一次旋转的上限时间
        rospy.loginfo("start")
        car_act = car_move
        if (car_act == [0, 0, 0]):
            rospy.loginfo("cricle")
            print("nothing found!!!!!!")
            # str = input("输入指令 = ")
            # if str == 'c':
            self.move(self.move_cricle)
            rospy.sleep(5)
            self.move(self.move_stop)
    # rospy.sleep(0.05)
    # t2 = time.time()
    # self.revolve_time = self.revolve_time + t2 - t1  # 累加旋转时间
        else:
            # self.move(self.move_stop)
            # rospy.sleep(5)
            # self.revolve_time = 0
            if (car_act[0] >= self.lb and car_act[0] <= self.rb):  # 如果在左右边界之间，经过一次确认后，方可捡球
                rospy.loginfo("left and right")
                self.move(self.move_forward)
                rospy.sleep(1.5)
                self.move(self.move_stop)
            # rospy.sleep(0.5)
            elif (car_act[0] < self.lb):  # 如果在左边界左边
                rospy.loginfo("left")
                self.move(self.move_left)
                i = (self.lb - car_act[0]) * self.dt
                rospy.sleep(i)
                self.move(self.move_stop)
            # rospy.sleep(0.5)
            else:  # 如果在右边界右边
                rospy.loginfo("right")
                self.move(self.move_right)
                i = (car_act[0] - self.rb) * self.dt
                rospy.sleep(i)
                self.move(self.move_stop)
        #rospy.sleep(10)


# self.move(self.move_stop)


# def move(self, data):
# 	self.twist.linear.x = data[0]; self.twist.linear.y = data[1]; self.twist.linear.z = data[2]
# 	self.twist.angular.x = data[3]; self.twist.angular.y = data[4]; self.twist.angular.z =data[5]
# 	self.image_pub.publish(self.twist)


if __name__ == "__main__":
    rospy.init_node("ping_subs")
    ping_sub()
    rospy.spin()