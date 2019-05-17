#!/usr/bin/env python
import rospy
from messages.msg import EnemyPos


def talker():
    gimbal_pub = rospy.Publisher('enemy_pos', EnemyPos, queue_size=1)
    rospy.init_node('gimbal_talker', anonymous=True)
    rate = rospy.Rate(2)  # 10hz
    cnt = 0
    # while not rospy.is_shutdown():
    #     gimbal_cmd = EnemyPos()
    #     line = raw_input(">")
    #     if(line == 'o'):
    #         gimbal_cmd.enemy_pitch = 0.18
    #     if(line == 'l'):
    #         gimbal_cmd.enemy_pitch = -0.18
    #     if(line == 'j'):
    #         gimbal_cmd.enemy_yaw = -0.18
    #     if(line == 'k'):
    #         gimbal_cmd.enemy_yaw = 0.18
    #     gimbal_pub.publish(gimbal_cmd)
    #     rate.sleep()
    while not rospy.is_shutdown():
        gimbal_cmd = EnemyPos()
        if cnt % 2 :
            gimbal_cmd.enemy_pitch = 0.0
            gimbal_cmd.enemy_yaw = 0.18
        else:
            gimbal_cmd.enemy_pitch = 0.0
            gimbal_cmd.enemy_yaw = -0.18
        gimbal_pub.publish(gimbal_cmd)
        cnt += 1
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass