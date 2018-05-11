#!/usr/bin/env python

import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from icra_firefly.msg import ArmorDetection
from cv_bridge import CvBridge
import cv2
from SSDDeal import SSDDeal


def callback(image_msg):
    _cv_bridge = CvBridge()
    cv_image = _cv_bridge.imgmsg_to_cv2(image_msg, "bgr8")
    cv2.imshow('xx', cv_image)
    rclasses, rscores, rbboxes = _ssd.process_image(cv_image)
    print rclasses
    for i in range(rbboxes.shape[0]):
        rscore = rscores[i]
        rclass = rclasses[i]
        rbbox  = rbboxes[i]
        rect_detection = ArmorDetection()
        if rclass == 1:
            rect_detection.kind = 'Car'
        if rclass == 2:
            rect_detection.kind = 'Armor'
        rect_detection.y1 = rbbox[0]
        rect_detection.x1 = rbbox[1]
        rect_detection.y2 = rbbox[2]
        rect_detection.x2 = rbbox[3] 
        _pub.publish(rect_detection)
        print rscore
    cv2.waitKey(20)


def listener():
    rospy.init_node('armor_detection_node', anonymous=True)

    rospy.Subscriber('image', Image, callback, queue_size=1)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    _ssd = SSDDeal()
    _rect_detection = ArmorDetection()
    _pub = rospy.Publisher('armor_detection', ArmorDetection, queue_size=10)
    listener()
