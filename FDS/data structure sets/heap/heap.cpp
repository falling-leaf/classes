//关于堆这一数据结构
//堆的第0位不存数据
//堆的size表示堆中的元素个数
//对的capacity表示堆可存储的最大元素个数
//注意！！！建堆是直接把数组赋值在堆里面，然后从第一个父节点（size/2）开始下渗
//堆的随机访问为O(1)，因此不存在遍历的问题
//将一些数据进行建堆-解堆的过程，输出的结果就是堆排序
//以min-heap为例
#include <bits/stdc++.h>
#define MAX_N 20
using namespace std;
struct Heap
{
    int data[MAX_N];//堆是以一维数组的方式呈现的
    int capacity;//最大值
    int size;//当前的堆大小，与vector有相似之处
};
Heap* oneheap = (Heap*)malloc(sizeof(Heap));

//initialize the heap
void initialize()
{
    oneheap->size = 0;
    oneheap->capacity = MAX_N;
}

//关键函数：percolate_up（上渗）
void percolate_up(int p)
{
    int temp;
    if (oneheap->data[p / 2] > oneheap->data[p])
    {
        temp = oneheap->data[p / 2];
        oneheap->data[p / 2] = oneheap->data[p];
        oneheap->data[p] = temp;
        percolate_up(p / 2);
    }
}

//关键函数：percolate_down（下渗）
void percolate_down(int p)
{
    if (p * 2 > oneheap->size)
        return;
    int temp, po;
    if (p * 2 + 1 <= oneheap->size && oneheap->data[p * 2 + 1] < oneheap->data[p * 2])
        temp = oneheap->data[p * 2 + 1], po = p * 2 + 1;
    else temp = oneheap->data[p * 2], po = p * 2;
    if (oneheap->data[p] > temp)
    {
        oneheap->data[po] = oneheap->data[p];
        oneheap->data[p] = temp;
        percolate_down(po);
    }
}

//关键函数：build（建堆）
void build(int n)
{
    for (int i = 1; i <= n; i++)
        cin >> oneheap->data[i];
    oneheap->size = n;
    for (int i = (oneheap->size) / 2; i >= 0; i--)
        percolate_down(i);
}

//关键函数：insert（将新数据记在数组末尾，然后依次上渗）
void insert()
{
    int newdata, p;
    cin >> newdata;
    p = ++oneheap->size;//oneheap->size++, p = oneheap->size
    oneheap->data[p] = newdata;
    percolate_up(p);
}

//关键函数：deletemin（将最小值删除，将堆的末尾移到堆的第一位，然后依次下渗）
void deletemin()
{
    cout << "delete " << oneheap->data[1] << endl;
    oneheap->data[0] = oneheap->data[oneheap->size--];//堆的末尾移到第一位，然后size--
    percolate_down(0);
}

int main()
{
    initialize();
    build(5);
    for (int i = 0; i < 5; i++)
        deletemin();
}