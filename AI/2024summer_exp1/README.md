# README
本程序为《人工智能前沿》lab1：手写数字识别的实现，文件结构如下：
- data & code: 数据集及程序代码
  - `analysis.py`: 对`acc_lst.txt`,`loss_lst.txt`数据进行分析
  - `train.py`: 对当前目录下的存档进行训练
  - `test.py`: 对当前目录下的存档进行测试，并生成正确率分析
  - `module.py`: 卷积神经网络内部结构
  - `acc_lst.txt`: 训练集上的正确率记录
  - `loss_lst.txt`: 训练集上的损失值记录
  - `result.txt`: 随机采样测试的记录
  - `result_lst.txt`: 测试集上的正确率记录
  - `model_parameters.pickle`: 当前权重矩阵存档
  - `savings`: 存储不同迭代次数后，对应测试集正确率的存档文件夹
- doc: 文档
  - report.md/report.pdf: 实验报告