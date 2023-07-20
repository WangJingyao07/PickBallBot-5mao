#! /usr/bin/env python
import rospy
import cv2
from image.msg import ping
import numpy as np

iLowH = 5
iHighH = 22

iLowS = 168
iHighS = 255

iLowV = 51
iHighV = 255

IMAGE_WINDOW_NAME = 'YelloBarTracker'
CONTROL_WINDOW_NAME = 'Control'
MASK_WINDOW_NAME = 'Mask'

def nothing():
    pass
def make_hsv_adjustment():
    cv2.namedWindow(CONTROL_WINDOW_NAME)
    cv2.createTrackbar('LowH', CONTROL_WINDOW_NAME, iLowH, 255, nothing) #Hue (0 - 179)
    cv2.createTrackbar('HighH', CONTROL_WINDOW_NAME, iHighH, 255, nothing)

    cv2.createTrackbar('LowS', CONTROL_WINDOW_NAME, iLowS, 255, nothing) #Saturation (0 - 255)
    cv2.createTrackbar('HighS', CONTROL_WINDOW_NAME, iHighS, 255, nothing)

    cv2.createTrackbar('LowV', CONTROL_WINDOW_NAME, iLowV, 255, nothing) #Value (0 - 255)
    cv2.createTrackbar('HighV', CONTROL_WINDOW_NAME, iHighV, 255, nothing)


def main():

    rospy.init_node("image_process", anonymous=True)
    ping_info_pub = rospy.Publisher('/ping_info', ping, queue_size=10)
    rate = rospy.Rate(10)
    msg = ping()
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    #make_hsv_adjustment()
    while cap.isOpened():
        ret, image0 = cap.read()

        if(ret):
            blur = cv2.GaussianBlur(image0, (5,5),0)
            hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

            # iLowH = cv2.getTrackbarPos('LowH', CONTROL_WINDOW_NAME)
            # iHighH = cv2.getTrackbarPos('HighH', CONTROL_WINDOW_NAME)
            # iLowS = cv2.getTrackbarPos('LowS', CONTROL_WINDOW_NAME)
            # iHighS = cv2.getTrackbarPos('HighS', CONTROL_WINDOW_NAME)
            # iLowV = cv2.getTrackbarPos('LowV', CONTROL_WINDOW_NAME)
            # iHighV = cv2.getTrackbarPos('HighV', CONTROL_WINDOW_NAME)

            lower_yellow = np.array([iLowH,iLowS,iLowV])
            upper_yellow = np.array([iHighH,iHighS,iHighV])

            mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
            # cv2.imshow(MASK_WINDOW_NAME, mask)
            bmask = cv2.GaussianBlur(mask, (5,5),0)
            circles = cv2.HoughCircles(
                    bmask,
                    cv2.HOUGH_GRADIENT,
                    1,
                    50,
                    param1=100,
                    param2=45,
                    minRadius=0,
                    maxRadius=0
                )
            best_circle = [0, 0, 0]
            if circles is None:
                rospy.loginfo("none")
                msg.x = 0
                msg.y = 0
                msg.r = 0
                ping_info_pub.publish(msg)
            else:
                circles = np.uint16(np.around(circles))
                print(circles)
                for i in circles[0, :]:
                    cv2.circle(image0, (i[0], i[1]), i[2], (0, 255, 0), 2)
                    cv2.circle(image0, (i[0], i[1]), 2, (0, 0, 255), 3)
                for i in circles[0, :]:
                    if (i[2] >= best_circle[2]):
                        best_circle = i.tolist()
                rospy.loginfo("publisher:   X:%d,Y:%d,R:%d",best_circle[0],best_circle[1],best_circle[2])
                msg.x = best_circle[0]
                msg.y = best_circle[1]
                msg.r = best_circle[2]
                ping_info_pub.publish(msg)
            cv2.imshow(IMAGE_WINDOW_NAME, image0)
            cv2.waitKey(10)
        rate.sleep()

if __name__ == '__main__':
    main()