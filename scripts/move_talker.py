import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist


def talker():
    cmd_pub = rospy.Publisher('cmd_vel', Twist, queue_size=1)
    rospy.init_node('move_talker', anonymous=True)
    rate = rospy.Rate(1)  # 100hz
    while not rospy.is_shutdown():
        move_cmd = Twist()
        line = raw_input(">")
        if(line == 'w'):
            move_cmd.linear.x = 0.1
        if(line == 's'):
            move_cmd.linear.x = -0.1
        if(line == 'a'):
            move_cmd.linear.y = 0.1
        if(line == 'd'):
            move_cmd.linear.y = -0.1
        if(line == 'q'):
            move_cmd.angular.z = -0.1
        if(line == 'e'):
            move_cmd.angular.z = 0.1
        if(line == 'z'):
            move_cmd.linear.y = 0
            move_cmd.linear.x = 0
            move_cmd.angular.z = 0
        cmd_pub.publish(move_cmd)
        rate.sleep()


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
