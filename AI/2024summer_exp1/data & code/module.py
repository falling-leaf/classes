import numpy as np
#import cupy as np
import math

class Conv2d:
    #在Conv2d中，我们需要实现对传入图像（x）的卷积操作
    def __init__(self, in_channels: int, out_channels: int, kernel_size: int, 
                 stride: int = 1, padding: int = 0, dtype = None):
        # ------------------------------请完成此部分内容------------------------------
        # 这里将所有需要用到的数据进行记录
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding
        self.dtype = dtype

        # 对于每个卷积核，有：图片高/图片宽/输入的通道数，这些综合起来形成一个卷积核，而这样的卷积核个数应当为out_channels
        # 在后向传播时，我们需要对它们进行调参

        stddev = 1.0 / np.sqrt(in_channels * kernel_size * kernel_size)

        self.cores_array = np.random.normal(0.0, stddev, (out_channels, in_channels, kernel_size, kernel_size)).astype(dtype)

        # ---------------------------------------------------------------------------

    def forward(self, x):
        """
        x - shape (N, C, H, W)
        return the result of Conv2d with shape (N, O, H', W')
        """
        # ------------------------------请完成此部分内容------------------------------
        # 首先，我们需要明确：在（初始）传入的x中，是一个60000x786的数据，即数据不是以2维数组的方式存储的，因此响应的卷积操作也应当进行改变
        rs_depth = self.out_channels
        cores_array = self.cores_array
        padding = self.padding
        kernel_size = self.kernel_size
        offset = kernel_size // 2
        # 在train中我们已经将数组变为统一格式，在这里直接对相应格式进行处理即可
        num, depth, row, col = x.shape
        if padding != 0:
            extend_x = np.pad(x, ((0, 0), (0, 0), (padding, padding), (padding, padding)), mode = 'constant', constant_values=0)
        else: extend_x = x
        row_out = extend_x.shape[2] - kernel_size + 1
        col_out = extend_x.shape[3] - kernel_size + 1
        rs_array = np.zeros((num, rs_depth, row_out, col_out))
        self.x = extend_x
        # 现在对extend_x进行卷积操作
        for i in range(num):
            for j in range(row_out):
                for k in range(col_out):
                    sub_matrix = extend_x[i, :, j:(j + kernel_size), k:(k + kernel_size)]
                    for l in range(rs_depth):
                        rs_array[i][l][j][k] = np.sum(sub_matrix * cores_array[l])
        return rs_array
    
        # ---------------------------------------------------------------------------

    def backward(self, dy, lr):
        """
        dy - the gradient of last layer with shape (N, O, H', W')
        lr - learning rate
        calculate self.w_grad to update self.weight,
        calculate self.b_grad to update self.bias,
        return the result of gradient dx with shape (N, C, H, W)
        """
        # ------------------------------请完成此部分内容------------------------------
        # 这里为求方便，我们假设padding是否存在都不影响读取位置（只是对矩阵进行扩充与否）
        # 考虑到做卷积时“图片数量”维度的影响，这里最终决定将图片数量统一纳入权重计算中，最后进行求平均值得到权重误差
        num_dy, depth_dy, row_dy, col_dy = dy.shape
        num_x, depth_x, row_x, col_x = self.x.shape
        if num_dy != num_x:
            raise ValueError("卷积数量出错")
        dw = np.zeros((depth_dy, depth_x, self.kernel_size, self.kernel_size))
        for i in range(depth_x):
            for j in range(depth_dy):
                for k in range(row_x - row_dy + 1):
                    for l in range(col_x - col_dy + 1):
                        submatrix = self.x[:, i, k:(k + row_dy), l:(l + col_dy)]
                        dw[j, i, k, l] = np.sum(submatrix * dy[:, j, :, :])
        # 现在dw就是偏移的权重，但先要将下一步传递的dx计算出来后再进行更新
        # 这里实质上是将卷积核看作depth_dy x kernel_size x kernel_size来对dy进行全卷积
        padding = self.kernel_size - 1 # 此处的padding为全连接的padding
        kernel_size = self.kernel_size
        dy = np.pad(dy, ((0, 0), (0, 0), (padding, padding), (padding, padding)), mode = 'constant', constant_values=0)
        dx = np.zeros((num_x, depth_x, row_x, col_x))
        filt_w = np.flip(self.cores_array, axis=(2, 3))
        for i in range(num_x):
            for j in range(depth_x):
                for k in range(row_x):
                    for l in range(col_x):
                        submatrix = dy[i, :, k:(k + kernel_size) ,l:(l + kernel_size)]
                        dx[i, j, k, l] = np.sum(submatrix * filt_w[:, j, :, :])
        self.cores_array = self.cores_array - lr * dw
        return dx

    
        # ---------------------------------------------------------------------------
    

     

class ReLU:
    def forward(self, x):
        
        x = np.maximum(x, 0)
        self.x = x
        return x
    def backward(self, dy):
        dy = np.where(self.x > 0, self.x, 0)
        return dy

class Tanh:
    def forward(self, x):
        return

    def backward(self, dy):
        return
    
class Sigmoid:
    def forward(self, x):
        # 这里需要将传入的x中每一个位置都经过sigmoid函数处理
        x = 1 / (1 + np.exp(-x))
        self.x = x
        return x
    def backward(self, dy):
        dy = dy * self.x * (1 - self.x)
        return dy

class MaxPool2d:
    def __init__(self, kernel_size: int, stride = None, padding = 0):
        return

    def forward(self, x):
        """
        x - shape (N, C, H, W)
        return the result of MaxPool2d with shape (N, C, H', W')
        """
       

    def backward(self, dy):
        """
        dy - shape (N, C, H', W')
        return the result of gradient dx with shape (N, C, H, W)
        """
     
class AvgPool2d:
    def __init__(self, kernel_size: int, stride = None, padding = 0):
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding
       
    def forward(self, x):
        """
        x - shape (N, C, H, W)
        return the result of AvgPool2d with shape (N, C, H', W')
        """
        # 这里只处理整数倍的东西了
        num, depth, row, col = x.shape
        kernel_size = self.kernel_size
        rs_array = np.zeros((num, depth, row // kernel_size, col // kernel_size)) # 结果是这个
        for i in range(num):
            for j in range(depth):
                point_x = 0
                point_y = 0
                while point_x < row:
                    while point_y < col:
                        sub_matrix = x[i, j, point_x:(point_x + kernel_size), point_y:(point_y + kernel_size)]
                        rs_array[i][j][point_x // kernel_size][point_y // kernel_size] = np.mean(sub_matrix)
                        point_y += kernel_size
                    point_y = 0
                    point_x += kernel_size
        self.x = rs_array
        return rs_array

    def backward(self, dy):
        """
        dy - shape (N, C, H', W')
        return the result of gradient dx with shape (N, C, H, W)
        """
        N, C, H, W = self.x.shape
        # 由于这里存在着直接将linear的输出传回来的可能，所以需要转化一下
        if dy.ndim != 4:
            dy = dy.reshape(N, C, H, W)
        #H_pad, W_pad = H + self.padding * 2, W + self.padding * 2
        K, stride = self.kernel_size, self.stride
        #outH, outW = (H_pad - K) // stride + 1, (W_pad - K) // stride + 1
        #if outH != dy.shape[2] or outW != dy.shape[3]:
        #    raise RuntimeError
        #dx = np.zeros((N, C, H_pad, W_pad))
        dx = np.zeros((N, C, H * K, W * K))
        for i in range(H):
            for j in range(W):
                dx[:, :, i * K:i * K + K, j * K: j * K + K] = dy[:, :, i:i + 1, j: j + 1] / K ** 2
        return dx
        #for i in range(outH):
        #    for j in range(outW):
        #        dx[:, :, i*stride:i*stride+K, j*stride:j*stride+K] += dy[:, :, i:i+1, j:j+1] / K**2
        #return dx[:, :, self.padding:self.padding+H, self.padding:self.padding+W]

       
        

class Linear:
    def __init__(self, in_features: int, out_features: int, bias: bool = True):
        self.in_features = in_features
        self.out_features = out_features

        # 如果需要开启bias，那么bias的格式应当为仅有out_features
        if bias:
            self.bias = np.random.randn(out_features)
            self.b_grad = np.zeros(out_features)
        else:
            self.bias = None

        # weight是要调整的参数集合，用它来计算：y = w * x + b
        # 初始化时，将它初始设置为random * sqrt(2 / in)
        # 在向量化操作中，通过[num, in] * [in, out]获得[num, out]从而得到处理
        self.weight = np.random.randn(in_features, out_features) * np.sqrt(2 / self.in_features)
        
        

    def forward(self, x):
        """
        x - shape (N, C)
        return the result of Linear layer with shape (N, O)
        """

        x = x.reshape(x.shape[0], -1)
        self.x = x # 为backward存储数据
        if x.shape[1] != self.in_features:
            raise ValueError("全连接通道数错误")
        rs_array = np.matmul(x, self.weight)
        # print(res.shape, self.bias.shape)
        if self.bias is not None:
            rs_array += self.bias
        return rs_array

       


    def backward(self, dy, lr):
        """
        dy - shape (N, O)
        return the result of gradient dx with shape (N, C)
        """
        N, O = dy.shape
        if self.out_features != O or self.x.shape[0] != N:
            raise ValueError("返回节点出错")
        w_grad = np.matmul(self.x.T, dy) / N # 此时，w_grad与weight格式一致
        if self.bias is not None:
            b_grad = np.sum(dy, axis=0) / N # 此时，b_grad与bias格式一致

        dx = np.matmul(dy, self.weight.T) # dx是往回传播的信息，相当于对x偏导

        # 更新该层的属性
        self.weight = self.weight - lr * w_grad
        if self.bias is not None:
            self.bias = self.bias - lr * b_grad
        return dx


def softmax(x):
    num, map = x.shape
    for i in range(num):
        x[i] = np.exp(x[i])
        x[i] = x[i] / np.sum(x[i])
    return x

class CrossEntropyLoss:
    def __call__(self, x, label):
        # 此时传入的必然是：x: [num * 10]; label: [num * 1]
        dx_shape = x.shape
        if len(x.shape) != 2:
            x = x.reshape(x.shape[0], -1)
        if len(label.shape) != 2:
            label = label.reshape(label.shape[0], -1)
        num, map = x.shape
        if label.shape[0] != num:
            raise ValueError("label数量出错")
        elif label.shape[1] == 1:
            z = np.zeros((num, map)) # 现在要将label改写为概率分布的形式
            # 例如如果结果是3，应当为[0,0,1,0,0,0,0,0,0,0]
            z[np.indices((num,)), label.reshape(num,)] = 1
            label = z
        else: 
            raise ValueError("label格式出错")
        
        # 现在，我们已经获取到了label对应的实际概率分布。我们现在需要通过x来计算预测的概率分布（通过softmax函数）
        # softmax：e^x/sum(e^x)
        x = softmax(x)
        # 现在的x就是我们预测出来的概率分布
        # 接下来，需要通过交叉熵的公式进行计算
        loss = -np.sum(label * np.log(x))
        accuracy = 0
        # 对于这里的dx,需要将它和softmax结合起来算
        for i in range(x.shape[0]):
            test = np.argmax(x[i])
            if label[i][test] == 1:
                accuracy += 1
        accuracy /= x.shape[0]
        #dy1 = -label / x#[num, 10]
        #dy2 = x * (1 - x)#[num, 10]
        #dy = dy1 * dy2
        dy = x - label
        return loss, accuracy, dy



'''
        if self.in_channels == 1:
            num, length = x.shape
            size = int(math.sqrt(length))
            if padding != 0:
                rs_array = np.zeros((num, rs_depth, length))
                for k in range(num):
                    for j in range(self.out_channels):
                        for i in range(length):
                            sub_sum = 0
                            cnt = 0
                            for r in range(-offset, offset + 1):
                                for s in range(-offset, offset + 1): # 此时遍历的次数与卷积核一致，因此直接用cnt来确定卷积核位置
                                    if s < 0:
                                        if (i % size) < ((i + s) % size):
                                            cnt += 1
                                            continue
                                    else:
                                        if (i % size) > ((i + s) % size):
                                            cnt += 1
                                            continue
                                    if i + s + r * size < 0 or i + s + r * size >= length: # 这个对上下界进行了判定
                                        cnt += 1 # 如果超限，那么赋值为0，直接跳过
                                        continue
                                    sub_sum += x[k][i + s + r * size] * cores_array[j][cnt][0]
                                    cnt += 1
                            rs_array[k][j][i] = sub_sum
            else: return
        else: # 此时padding = 0
            num, depth, length = x.shape # 由于多层的x只会在中间出现，因此内部消化掉这个定义就好
            size = int(math.sqrt(length))
            if padding == 0: # 需要加padding
                rs_array = np.zeros((num, rs_depth, (size - kernel_size + 1) ** 2))
                for k in range(num):
                    for j in range(self.out_channels):
                        rs_cnt = 0
                        for i in range(length):
                            if i < size or i >= size * (size - 1) or i % size == 0 or i % size == (size - 1):
                                continue
                            sub_sum = 0
                            cnt = 0
                            for r in range(-offset, offset + 1):
                                for s in range(-offset, offset + 1): # 此时遍历的次数与卷积核一致，因此直接用cnt来确定卷积核位置
                                    for d in range(depth):
                                        sub_sum += x[k][d][i + s + r * size] * cores_array[j][cnt][d]
                                    cnt += 1
                            rs_array[k][j][rs_cnt] = sub_sum
                            rs_cnt += 1
            else: return
        return rs_array

    # 当具备padding时，对它进行拓展
        #if padding != 0:
        #    rs_array = np.zeros((num, rs_depth, row, col))
        #    extend_x = np.zeros((num, depth, row + 2 * padding, col + 2 * padding))
        #    for i in range(num):
        #        for j in range(depth):
        #            for k in range(padding, row + padding):
        #                extend_x[i][j][k][padding:(col + padding)] = x[i][j][k - padding][:]
        #else: 
        #    rs_array = np.zeros((num, rs_depth, row - kernel_size + 1, col - kernel_size + 1))
        #    extend_x = x
'''