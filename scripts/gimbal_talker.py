import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from icra_firefly.msg import GimbalControl


def talker():
    gimbal_pub = rospy.Publisher('gimbal', GimbalControl, queue_size=1)
    rospy.init_node('gimbal_talker', anonymous=True)
    rate = rospy.Rate(1)  # 100hz
    while not rospy.is_shutdown():
        gimbal_cmd = GimbalControl()
        line = raw_input(">")
        if(line == 'o'):
            gimbal_cmd.pitch = 0.1
        if(line == 'l'):
            gimbal_cmd.pitch = -0.1
        if(line == 'j'):
            gimbal_cmd.yaw = -0.1
        if(line == 'k'):
            gimbal_cmd.yaw = 0.1
        gimbal_pub.publish(gimbal_cmd)
        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass