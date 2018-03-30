#!/usr/bin/env python
# Software License Agreement (BSD License)
#
# Copyright (c) 2018, Robin Hanxy.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Revision $Id$

## Simple talker demo that published icra_firefly/ArmorDetection messages
## to the 'armor_detection

import rospy
import cv2


from SSDDeal import SSDDeal
from sensor_msgs.msg import Image
from icra_firefly.msg import ArmorDetection
from cv_bridge import CvBridge

class ArmorDetect(object):
    def __init__(self):
	self._cv_bridge = CvBridge()
	
	self._sub = rospy.Subscriber('image', Image, self.callback, queue_size=10)
	self._pub = rospy.Publisher('armor_detection', ArmorDetection, queue_size=10)
	print 'before ssd'
	self._ssd = SSDDeal()
	print 'after ssd'
	self._threshold = 0.3

    def callback(self, image_msg):
	cv_image = self._cv_bridge.imgmsg_to_cv2(image_msg, "bgr8")
	print 'wangchaobo sucker'
	rclasses, rscores, rbboxes = self._ssd.Deal_image(cv_image)
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
	    rect_detection.x1 = rbbox[0]
	    rect_detection.y1 = rbbox[1]
	    rect_detection.x2 = rbbox[2]
	    rect_detection.y2 = rbbox[3] 
            self._pub.publish(rect_detection)
	
    def main(self):
	rospy.spin()

if __name__ == '__main__':
    rospy.init_node('armor_detection_demo')
    armor_detection = ArmorDetect()
    armor_detection.main()
