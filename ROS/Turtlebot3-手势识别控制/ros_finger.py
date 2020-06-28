#!/usr/bin/env python
import sys
import cv2
import numpy as np
import copy
import math
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image, CompressedImage
import rospy
import roslib
from geometry_msgs.msg import Twist
# roslib.load_manifest('my_package')
# from appscript import app

# Environment:
# OS    : Mac OS EL Capitan
# python: 3.5
# opencv: 2.4.13

# parameters

learningRate = 0


def printThreshold(thr):
    print("! Changed threshold to "+str(thr))


def removeBG(bgModel,frame):
    fgmask = bgModel.apply(frame, learningRate=learningRate)
    # kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (3, 3))
    # res = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel)

    kernel = np.ones((3, 3), np.uint8)
    fgmask = cv2.erode(fgmask, kernel, iterations=1)
    res = cv2.bitwise_and(frame, frame, mask=fgmask)
    return res


def calculateFingers(res, drawing):  # -> finished bool, cnt: finger count
    #  convexity defect
    hull = cv2.convexHull(res, returnPoints=False)
    if len(hull) > 3:
        defects = cv2.convexityDefects(res, hull)
        if type(defects) != type(None):  # avoid crashing.   (BUG not found)

            cnt = 0
            for i in range(defects.shape[0]):  # calculate the angle
                s, e, f, d = defects[i][0]
                start = tuple(res[s][0])
                end = tuple(res[e][0])
                far = tuple(res[f][0])
                a = math.sqrt((end[0] - start[0]) ** 2 +
                              (end[1] - start[1]) ** 2)
                b = math.sqrt((far[0] - start[0]) ** 2 +
                              (far[1] - start[1]) ** 2)
                c = math.sqrt((end[0] - far[0]) ** 2 + (end[1] - far[1]) ** 2)
                angle = math.acos((b ** 2 + c ** 2 - a ** 2) /
                                  (2 * b * c))  # cosine theorem
                if angle <= math.pi / 2:  # angle less than 90 degree, treat as fingers
                    cnt += 1
                    cv2.circle(drawing, far, 8, [211, 84, 0], -1)
            return True, cnt
    return False, 0


class image_converter:

    def __init__(self):
        self.isBgCaptured = False
        # self.image_pub = rospy.Publisher("image_topic_2", Image)
        self.bridge = CvBridge()
        self.camera = cv2.VideoCapture(0)
        self.camera.set(10,100)
        rospy.Timer(rospy.Duration(0.1), self.callback, oneshot=False)
        # self.image_sub = rospy.Subscriber(
        #     "/image_raw/compressed", CompressedImage, self.callback)
        self.cmd_vel = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=5)
        self.triggerSwitch = False  # if true, keyborad simulator works
        self.cap_region_x_begin = 0.5  # start point/total width
        self.cap_region_y_end = 0.8  # start point/total width
        self.threshold = 90  # BINARY threshold
        self.blurValue = 41  # GaussianBlur parameter
        self.bgSubThreshold = 50
        self.bgModel = None
        self.drawing = None

    def sendtwist(self, n):
        move_cmd = Twist()
        if n <= 0:
            move_cmd.linear.x = 0
            move_cmd.angular.z = 0
            print("Stop")
        elif n <= 1:
            move_cmd.linear.x = 1
            move_cmd.angular.z = 0
            print("Forward")
        elif n <= 2:
            move_cmd.linear.x = 0
            move_cmd.angular.z = 1
            print("Turn")
        elif n <= 5:
            move_cmd.linear.x = 1
            move_cmd.angular.z = 1
            print("Circle")
        else:
            move_cmd.linear.x = 0
            move_cmd.angular.z = 0
        self.cmd_vel.publish(move_cmd)

    def callback(self, data):

        cv2.namedWindow('trackbar')
        cv2.namedWindow('original')
        cv2.createTrackbar('trh1', 'trackbar', self.threshold, 100, printThreshold)
        # try:
        #     frame = self.bridge.compressed_imgmsg_to_cv2(data, "bgr8")
        # except CvBridgeError as e:
        #     print(e)
        ret, frame = self.camera.read()
        self.threshold = cv2.getTrackbarPos('trh1', 'trackbar')
        frame = cv2.bilateralFilter(frame, 5, 50, 100)  # smoothing filter
        frame = cv2.flip(frame, 1)  # flip the frame horizontally
        cv2.rectangle(frame, (int(self.cap_region_x_begin * frame.shape[1]), 0),
                    (frame.shape[1], int(self.cap_region_y_end * frame.shape[0])), (255, 0, 0), 2)
        cv2.imshow('original', frame)

        #   Main operation
        if self.isBgCaptured == True:  # this part wont run until background captured
            img = removeBG(self.bgModel, frame)
            img = img[0:int(self.cap_region_y_end * frame.shape[0]),
                    int(self.cap_region_x_begin * frame.shape[1]):frame.shape[1]]  # clip the ROI
            # cv2.imshow('mask', img)

            # convert the image into binary image
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            blur = cv2.GaussianBlur(gray, (self.blurValue, self.blurValue), 0)
            # cv2.imshow('blur', blur)
            ret, thresh = cv2.threshold(blur, self.threshold, 255, cv2.THRESH_BINARY)
            # cv2.imshow('ori', thresh)

            # get the coutours
            thresh1 = copy.deepcopy(thresh)
            _, contours, hierarchy = cv2.findContours(
                thresh1, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            length = len(contours)
            maxArea = -1
            if length > 0:
                for i in range(length):  # find the biggest contour (according to area)
                    temp = contours[i]
                    area = cv2.contourArea(temp)
                    if area > maxArea:
                        maxArea = area
                        ci = i

                res = contours[ci]
                hull = cv2.convexHull(res)
                self.drawing = np.zeros(img.shape, np.uint8)
                cv2.drawContours(self.drawing, [res], 0, (0, 255, 0), 2)
                cv2.drawContours(self.drawing, [hull], 0, (0, 0, 255), 3)

                isFinishCal, cnt = calculateFingers(res, self.drawing)
                if self.triggerSwitch is True:
                    if isFinishCal is True:
                        self.sendtwist(cnt)
                        #print (cnt)
                        # app('System Events').keystroke(' ')  # simulate pressing blank space

            cv2.imshow('trackbar', self.drawing)

        # # Keyboard OP
        k = cv2.waitKey(3)
        if k == ord('b'):  # press 'b' to capture the background
            self.bgModel = cv2.createBackgroundSubtractorMOG2(0, self.bgSubThreshold)
            self.triggerSwitch = True
            self.isBgCaptured = True
            print('!!!Background Captured!!!')
        elif k == ord('r'):  # press 'r' to reset the background
            self.bgModel = None
            self.triggerSwitch = False
            self.isBgCaptured = False
            print ('!!!Reset BackGround!!!')
        # elif k == ord('n'):
        #     self.triggerSwitch = True
        #     print ('!!!Trigger On!!!')
        #     cv2.waitKey(3)


def main(args):
    rospy.init_node('Gesture_recogination', anonymous=False)
    ic = image_converter()
    print ('STRAT')
    
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    cv2.destroyAllWindows()
   
if __name__ == '__main__':
    main(sys.argv)
