# encoding=UTF-8
from nets import ssd_vgg_300
from nets import np_methods
from preprocessing import ssd_vgg_preprocessing
import tensorflow as tf
import numpy as np
import cv2

slim = tf.contrib.slim


class SSDDeal:

    def __init__(self):
        self.isess = tf.InteractiveSession()
        self.num_classes = 3
        self.ckpt_filename = 'ckpts/model.ckpt-38804'
        params = ssd_vgg_300.SSDNet.default_params._replace(num_classes=self.num_classes)
        ssd = ssd_vgg_300.SSDNet(params)
        self.net_shape = (300, 300)
        self.img_input = tf.placeholder(tf.uint8, shape=(None, None, 3))
        self.layers_anchors = ssd.anchors(self.net_shape, dtype=np.float32)
        image_pre, labels_pre, bboxes_pre, self.bbox_img = ssd_vgg_preprocessing.preprocess_for_eval(
            self.img_input,
            None,
            None,
            self.net_shape)
        self.image_4d = tf.expand_dims(image_pre, 0)
        arg_scope = ssd.arg_scope(weight_decay=0.00004)
        with slim.arg_scope(arg_scope):
            # 定义ssd网络
            self.predictions, self.localisations, logits, end_points = \
                ssd.net(self.image_4d, is_training=False)
        init_op = tf.global_variables_initializer()
        self.isess.run(init_op)
        # Restore SSD model.
        saver = tf.train.Saver()
        saver.restore(self.isess, self.ckpt_filename)

    def process_image(self, img, select_threshold=0.3, nms_threshold=0.35):
        # Run SSD network.
        rimg, rpredictions, rlocalisations, rbbox_img = self.isess.run([self.image_4d, self.predictions, self.localisations, self.bbox_img],
                                                                       feed_dict={self.img_input: img})
        rclasses, rscores, rbboxes = np_methods.ssd_bboxes_select(
            rpredictions, rlocalisations, self.layers_anchors,
            select_threshold=select_threshold, img_shape=self.net_shape, num_classes=self.num_classes, decode=True)
        rbboxes = np_methods.bboxes_clip(rbbox_img, rbboxes)
        rclasses, rscores, rbboxes = np_methods.bboxes_sort(rclasses, rscores, rbboxes,
                                                            top_k=400)
        rclasses, rscores, rbboxes = np_methods.bboxes_nms(rclasses, rscores, rbboxes, nms_threshold=nms_threshold)
        # Resize bboxes to original image shape.
        rbboxes = np_methods.bboxes_resize(rbbox_img, rbboxes)
        return rclasses, rscores, rbboxes

    def Deal_image(self, img):
        """
        Deal image to find car and armor
        :param img: the image to detection
        :return rclasses: shape is 1D list. 1 mean is car and 2 mean armor
        :return rscores: shape is 1D list. rscores[x] mean the confidence of rclasses[x]
        :return rbboxes: shape is 2D list. rbboxes[x] mean the bounding box of rclasses[x] and rbboxes[x] is 1D 4lenth list.
        :return rbboxes: first and second element mean p1 the Vertex of the bounding box.
        :return rbboxes: forth and fifth element mean the Vertex of the bounding box opposite to pt1.

        """
        rclasses, rscores, rbboxes = self.process_image(img)
        return rclasses, rscores, rbboxes


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
