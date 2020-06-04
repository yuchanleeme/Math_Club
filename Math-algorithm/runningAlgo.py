from fastai import *
from fastai.vision import *

class runningAlgo:
    def __init__(self, fileName):
        self.ans = self.algoFunc(fileName)

    def algoFunc(self, fileName):
        image_load_size = 64
        bs = 24

        tfms = get_transforms(do_flip=True, flip_vert=False, max_lighting=0.1, max_zoom=1.05,
                              max_warp=0.,
                              xtra_tfms=[rand_crop(), rand_zoom(1, 1.5),
                                         symmetric_warp(magnitude=(-0.2, 0.2))])

        data = (ImageList.from_folder(path='final_output_images')
                .split_by_folder(train='train',valid='val')
                .label_from_folder()
                .transform(tfms,size = image_load_size,resize_method=ResizeMethod.SQUISH)
                .databunch(path='.',bs=bs)
                .normalize(imagenet_stats)
        )

        model = cnn_learner(data,models.densenet161, metrics = [accuracy,error_rate])

        model.load('MYMODEL')

        import cv2 as cv
        import numpy as np
        img_color = cv.imread(fileName)
        img_gray = cv.cvtColor(img_color, cv.COLOR_BGR2GRAY)
        ret, img_binary = cv.threshold(img_gray, 0, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
        kernel = cv.getStructuringElement(cv.MORPH_RECT, (5, 5))
        img_binary = cv.morphologyEx(img_binary, cv.MORPH_CLOSE, kernel)
        contours, hierarchy = cv.findContours(img_binary, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

        con_array = []

        for contour in contours:
            x, y, w, h = cv.boundingRect(contour)

            img_dim = img_color[y:y + h, x:x + w]

            img_fastai = Image(pil2tensor(img_dim, dtype=np.float32).div_(255))
            img_fastai.resize(45)
            prediction = model.predict(img_fastai)
            con_array.append([x, y, w, h, str(prediction[0])])

        num_array = []
        final_out = ""
        con_array.sort(reverse = False)
        for idx, i in enumerate(con_array):
            num_array.append(i[-1])
            final_out += num_array[idx]

        return final_out
