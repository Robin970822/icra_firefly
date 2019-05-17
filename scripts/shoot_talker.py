#!/usr/bin/env python
import rospy
from std_msgs.msg import UInt8


def talker():
    shoot_pub = rospy.Publisher('shoot', UInt8, queue_size=1)
    rospy.init_node('shoot_talker', anonymous=True)
    rate = rospy.Rate(1)  # 100hz
    while not rospy.is_shutdown():
        line = raw_input(">")
        if(line == 'r'):
            shoot_cmd = 1
        shoot_pub.publish(shoot_cmd)
        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass