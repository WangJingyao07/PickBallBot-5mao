#! /usr/bin/env python
# encoding: utf-8
import rospy
import cv2
import math
import numpy as np
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from image_process.msg import ping

class Image_Process:
    def __init__(self):
        self.kernel = np.ones((5, 5), np.uint8)
        self.Low_H =5
        self.High_H=26
        self.Low_S = 110
        self.High_S = 245
        self.Low_V = 154
        self.High_V = 255
        self.Image_Window_Name="YelloBarTracker"
        self.Control_Window_Name='Control'
        self.Mask_Window_Nane='Mask'
        self.lower_yellow = np.array([self.Low_H,self.Low_S,self.Low_V])
        self.upper_yellow = np.array([self.High_H,self.High_S,self.High_V])
        self.ok_image_msg = Image()
        self.image_bridge = CvBridge()
        #乒乓球的坐标信息
        self.x = 0
        self.y = 0
        self.r = 0
        self.okimage_pub = rospy.Publisher('/ok/image', Image, queue_size=1)
        self.image_pub = rospy.Publisher('/ping_info', ping, queue_size=1)
        self.ping_msg = ping()
        rospy.init_node("image_process", anonymous=True)  #先初始化
        self.rate = rospy.Rate(45) #然后设置参数
        rospy.Subscriber("/camera/rgb/image_raw", Image, self.ImageCallback)

    def ImageCallback(self,msg):
        image = self.image_bridge.imgmsg_to_cv2(msg,'bgr8')
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, self.lower_yellow, self.upper_yellow)
        e_mask = cv2.erode(mask, self.kernel, iterations=1)
        p_mask = cv2.dilate(e_mask, self.kernel, iterations=1)
        binary,contours, hierarchy = cv2.findContours(p_mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        areas = [cv2.contourArea(contours[i]) for i in range(len(contours))]
        contour = list()
        if len(areas) != 0:
            contour.append(contours[areas.index(max(areas))])
            self.x,self.y = (int(np.mean(contour[0][:,0,0])),int(np.mean(contour[0][:,0,1])))
        else:
            self.x,self.y = 0,0
        cv2.drawContours(image, contour, -1, (0, 0, 255), 3)
        self.okimage_pub.publish(self.image_bridge.cv2_to_imgmsg(image,'bgr8'))
        cv2.imshow('image', image)
        cv2.waitKey(1)
    def nothing(self):
        pass
    def make_hsv_adjustment(self):
        cv2.namedWindow(self.Control_Window_Name)
        cv2.resizeWindow(self.Control_Window_Name,640,360)
        cv2.createTrackbar('Low_H', self.Control_Window_Name, self.Low_H, 255, self.nothing)
        cv2.createTrackbar('High_H', self.Control_Window_Name, self.High_H, 255, self.nothing)
        cv2.createTrackbar('Low_S', self.Control_Window_Name, self.Low_S, 255, self.nothing)
        cv2.createTrackbar('High_S', self.Control_Window_Name, self.High_S, 255, self.nothing)
        cv2.createTrackbar('Low_V', self.Control_Window_Name, self.Low_V, 255, self.nothing)
        cv2.createTrackbar('High_V', self.Control_Window_Name, self.High_V, 255, self.nothing)
    def main(self):
        self.make_hsv_adjustment()
        while not rospy.is_shutdown():
            self.Low_H = cv2.getTrackbarPos('Low_H', self.Control_Window_Name)
            self.High_H = cv2.getTrackbarPos('High_H', self.Control_Window_Name)
            self.Low_S = cv2.getTrackbarPos('Low_S', self.Control_Window_Name)
            self.High_S = cv2.getTrackbarPos('High_S', self.Control_Window_Name)
            self.Low_V = cv2.getTrackbarPos('Low_V', self.Control_Window_Name)
            self.High_V = cv2.getTrackbarPos('High_V', self.Control_Window_Name)
            self.lower_yellow = np.array([self.Low_H,self.Low_S,self.Low_V])
            self.upper_yellow = np.array([self.High_H,self.High_S,self.High_V])
            self.rate.sleep()
if __name__ == '__main__':
    process = Image_Process()
    process.main()
