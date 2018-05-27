#!
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist


def talker():
    cmd_pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
    rospy.init_node('fake_talker', anonymous=True)
    rate = rospy.Rate(50)  # 100hz
    while not rospy.is_shutdown():
        fake_cmd = Twist()
        fake_cmd.linear.z = 1.0
        cmd_pub.publish(fake_cmd)
        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
