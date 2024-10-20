import matplotlib.pyplot as plt  
loss_lst = []
acc_lst = []

with open('acc_lst.txt', 'r') as file:
    while True:
        line = file.readline()
        if len(line) > 0:
            temp_lst = eval(line)
        if not line:
            break
        acc_lst += temp_lst

x = [i * 100 for i in range(len(acc_lst))]
y = [i for i in acc_lst]
plt.figure()
plt.plot(x, y)  
plt.ylim(0, 1) 
# 添加标题和坐标轴标签  
plt.title('Simple Plot')  
plt.xlabel('x axis')  
plt.ylabel('y axis')  
  
# 显示网格  
plt.grid(True)  
  
# 显示图形  
plt.show()
  
with open('loss_lst.txt', 'r') as file:
    while True:
        line = file.readline()
        if len(line) > 0:
            temp_lst = eval(line)
        if not line:
            break
        loss_lst += temp_lst
x = [i * 100 for i in range(len(loss_lst))]
y = [i for i in loss_lst]
plt.figure()
plt.plot(x, y)  
# 添加标题和坐标轴标签  
plt.title('Simple Plot')  
plt.xlabel('x axis')  
plt.ylabel('y axis')  
  
# 显示网格  
plt.grid(True)  
  
# 显示图形  
plt.show()


