# Simple talker demo that published icra_firefly/ArmorDetection messages
# to the 'armor_detection

import rospy
from icra_firefly.msg import ArmorDetection


def talker():
    pub = rospy.Publisher('armor_detection', ArmorDetection, queue_size=100)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10)  # 10hz
    while not rospy.is_shutdown():
        time_str = "Time %s" % rospy.get_time()
        rospy.loginfo(time_str)
        # make you code here
        # make a while loop
        rect_detection = ArmorDetection()
        rect_detection.kind = 'Armor'
        rect_detection.x1 = 500.
        rect_detection.y1 = 60.
        rect_detection.x2 = 10.
        rect_detection.y2 = 10.
        pub.publish(rect_detection)
        # where you code ends
        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
