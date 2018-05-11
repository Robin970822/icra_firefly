import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from icra_firefly.msg import ArmorDetection
from cv_bridge import CvBridge
import cv2
from SSDDeal import SSDDeal

_ssd = SSDDeal()
_rect_detection = ArmorDetection()
_pub = rospy.Publisher('armor_detection', ArmorDetection, queue_size=10)
cap = cv2.VideoCapture(0)
while(cap.isOpened()):
    ret, frame = cap.read()
    if ret == True:
        cv2.imshow('cv_image', frame)
        rclasses, rscores, rbboxes = _ssd.process_image(frame)
        print rclasses
        for i in range(rbboxes.shape[0]):
            rscore = rscores[i]
            rclass = rclasses[i]
            rbbox = rbboxes[i]
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
        cv2.waitKey(20)
    else:
        break

cap.release()
cv2.destroyAllWindows()
