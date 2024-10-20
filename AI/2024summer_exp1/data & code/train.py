import numpy as np
#import cupy as cp
from module import Conv2d, Sigmoid, MaxPool2d, AvgPool2d, Linear, ReLU, Tanh, CrossEntropyLoss
import time
import struct
import pickle
import glob
import random
import os
import math
#import tqdm
#import cv2

def load_mnist(path, kind='train'):
    if kind == 'train':
        image_path = "train-images-idx3-ubyte"
        label_path = "train-labels-idx1-ubyte"
    else:
        image_path = "t10k-images-idx3-ubyte"
        label_path = "t10k-labels-idx1-ubyte"
    #image_path = glob.glob('./%s*3-ubyte' % (kind))[0]
    #label_path = glob.glob('./%s*1-ubyte' % (kind))[0]

    with open(label_path, "rb") as lbpath:
        magic, n = struct.unpack('>II', lbpath.read(8))
        labels = np.fromfile(lbpath, dtype=np.uint8)

    with open(image_path, "rb") as impath:
        magic, num, rows, cols = struct.unpack('>IIII', impath.read(16))
        images = np.fromfile(impath, dtype=np.uint8).reshape(len(labels), 28*28)

    return images, labels # 这里返回的是图片以及对应的标签

class LeNet5:
    def __init__(self):
        #传入的是28x28
        self.conv1 = Conv2d(1, 6, 5, 1, 2)#第一次卷积，卷积核大小为5，padding=2，处理后仍为28*28
        self.relu1 = Sigmoid()#使用激活函数
        self.pool1 = AvgPool2d(2)#平均池化，大小为2，处理后为14*14
        self.conv2 = Conv2d(6, 16, 5)#第二次卷积，卷积核大小为5，无padding，处理后为10*10
        #self.relu2 = Sigmoid()#使用激活函数
        self.relu2 = Sigmoid()#使用激活函数
        self.pool2 = AvgPool2d(2)#平均池化，大小为2，处理后为5*5
        self.fc1 = Linear(16*5*5, 120)#现在有16个out_channel,每个大小为5*5
        self.relu3 = Sigmoid()
        self.fc2 = Linear(120, 84)
        self.relu4 = Sigmoid()
        self.fc3 = Linear(84, 10)
    
    def write_weight(self, q, i):
        model_parameters = {
            'W1': self.conv1.cores_array,
            'W2': self.conv2.cores_array,
            'W3': self.fc1.weight,
            'b3': self.fc1.bias,
            'W4': self.fc2.weight,
            'b4': self.fc2.bias,
            'W5': self.fc3.weight,
            'b5': self.fc3.bias,
            "epoch": q,
            "data": i,
            }
        with open('model_parameters.pickle', 'wb') as f:
            pickle.dump(model_parameters, f)

    def read_weight(self):
        try:
            with open("model_parameters.pickle", "rb") as f:
                loaded = pickle.load(f)
                self.conv1.cores_array = loaded["W1"]
                self.conv2.cores_array = loaded["W2"]
                self.fc1.weight = loaded["W3"]
                self.fc1.bias = loaded["b3"]
                self.fc2.weight = loaded["W4"]
                self.fc2.bias = loaded["b4"]
                self.fc3.weight = loaded["W5"]
                self.fc3.bias = loaded["b5"]
                return loaded["epoch"], loaded["data"]
        except FileNotFoundError:
            return 0, 0

    def forward(self, x):
        x = self.conv1.forward(x)
        x = self.relu1.forward(x)
        x = self.pool1.forward(x)
        x = self.conv2.forward(x)
        x = self.relu2.forward(x)
        x = self.pool2.forward(x)
        x = self.fc1.forward(x)
        x = self.relu3.forward(x)
        x = self.fc2.forward(x)
        x = self.relu4.forward(x)
        x = self.fc3.forward(x)
        return x

    def backward(self, dy, lr):
        dy = self.fc3.backward(dy, lr)
        dy = self.relu4.backward(dy)
        dy = self.fc2.backward(dy, lr)
        dy = self.relu3.backward(dy)
        dy = self.fc1.backward(dy, lr)
        dy = self.pool2.backward(dy)
        dy = self.relu2.backward(dy)
        dy = self.conv2.backward(dy, lr)
        dy = self.pool1.backward(dy)
        dy = self.relu1.backward(dy)
        dy = self.conv1.backward(dy, lr)


def image_convert(x):
    num, length = x.shape
    size = int(math.sqrt(length))
    rs_array = np.zeros((num, 1, size, size))
    for i in range(num):
        for j in range(size):
            rs_array[i][0][j][:] = x[i][(j * size) : ((j + 1) * size)]
    return rs_array

if __name__ == '__main__':
    
    train_images, train_labels = load_mnist("MNIST-Dataset", kind="train")
    test_images, test_labels = load_mnist("MNIST-Dataset", kind="t10k")

    train_images = train_images.astype(np.float16) / 256
    test_images = test_images.astype(np.float16) / 256
    # ----------------------------请完成网络的训练和测试----------------------------
    # 现在train_images和test_images都是一维的，我们在这里直接将它转化为需要的格式，避免混乱
    # 输入：train_images(60000, 784) 输出：train_images(60000 x 1 x 28 x 28)
    train_images = image_convert(train_images)
    test_images = image_convert(test_images)
    cnn = LeNet5()
    epochs, datas = cnn.read_weight()
    cnt = 0
    losscheck = CrossEntropyLoss()

    for q in range(epochs, 20):
        cnt = 0
        acc_lst = []
        loss_lst = []
        for i in range(600):
            if q == epochs and i * 100 < datas:
                continue
            batch_images = train_images[cnt:(cnt + 100)]
            batch_labels = train_labels[cnt:(cnt + 100)]
            start_time = time.time()
            predict_rs = cnn.forward(batch_images)
            loss, accuracy, dy = losscheck(predict_rs, batch_labels)
            print("batch:", q * 60000 + i * 100, "loss:", loss, "accuracy:", accuracy)
            loss_lst.append(loss)
            acc_lst.append(accuracy)
            lr = 0.1
            cnn.backward(dy, lr)
            end_time = time.time()
            print("processing time: {:.3f}s".format(end_time - start_time))
            if i % 50 == 0:
                offset = random.randint(0, 9899)
                predict_rs = cnn.forward(test_images[offset:100 + offset])
                loss, accuracy, dy = losscheck(predict_rs, test_labels[offset:100 + offset])
                print("testing for iteration", q, "data", i * 100, "loss:", loss, "accuracy:", accuracy)
                with open('result.txt', 'a') as file:
                    file.write("testing for iteration {:} data {:}: loss: {:} accuracy: {:}\n".format(q, i * 100, loss, accuracy))
                with open('acc_lst.txt', 'a') as file:
                    file.write(str(acc_lst) + "\n")
                with open('loss_lst.txt', 'a') as file:
                    file.write(str(loss_lst) + "\n")
                loss_lst = []
                acc_lst = []
                cnn.write_weight(q, i * 100)
            cnt += 100
    
    
    # ---------------------------------------------------------------------------
    
    

    
            
            

