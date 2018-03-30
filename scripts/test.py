from SSDDeal import SSDDeal
import cv2
import numpy as np


def bboxes_draw_on_img(img, classes, scores, bboxes, colors, thickness=2):
    shape = img.shape
    for i in range(bboxes.shape[0]):
        bbox = bboxes[i]
        color = colors[classes[i]]
        # Draw bounding box...
        p1 = (int(bbox[0] * shape[0]), int(bbox[1] * shape[1]))
        p2 = (int(bbox[2] * shape[0]), int(bbox[3] * shape[1]))
        cv2.rectangle(img, p1[::-1], p2[::-1], color, thickness)
        # Draw text...
        s = '%s/%.3f' % (classes[i], scores[i])
        p1 = (p1[0] - 5, p1[1])
        cv2.putText(img, s, p1[::-1], cv2.FONT_HERSHEY_DUPLEX, 0.4, color, 1)


if __name__ == '__main__':
    ssd = SSDDeal()
    colors_tableau = [(255, 255, 255), (31, 119, 180), (174, 199, 232), (255, 127, 14)]
    for xstep in range(300, 40000):
        num = str(xstep)
        num = num.zfill(4)
        filename = '/home/hanxy/SSD-Tensorflow-master/icra/data_xmlJPEGImages/car' + num + '.jpg'
        img = cv2.imread(filename)
        rclasses, rscores, rbboxes = ssd.process_image(img)

        # Draw results.
        img_bboxes = np.copy(img)
        bboxes_draw_on_img(img_bboxes, rclasses, rscores, rbboxes, colors_tableau, thickness=2)
        cv2.imshow("xx", img_bboxes)
        cv2.waitKey(20)
