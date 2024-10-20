//调试日志：
//在boxsize与testsize大小关系中有问题
//无法调用拷贝函数
//当生成一维数组时，在任何情况下可成立,resize函数可以正常使用
//当生成二维数组时：
    //boxsize < testsize: 构造函数出现segment fault
        //解决方案：这是因为申请的内存不够  
        //考量到这个时候必须分配连续的内存，因此不可能使用离散的内存块来解决问题
        //因此不得不将这片区域用一般的方法处理掉
        //只能这么做，否则boxsize设的更大就是更好的解决方案
    //boxsize > testsize: 无错误

    //可能问题：vector(size)是什么意思？
//第二次调试唯一问题：在测试末尾，调用析构函数出现问题
//第三次调试问题：在allocate函数中，参量n表示调用的空间数，但在找freelist的时候，freelist是离散的空间
    //同时更不好的消息：resize()函数会令n为非1的数，这意味着需要解决的问题更多更难
    //现在经过测试，已知在申请总空间相同时，申请所需时间大致只与申请次数成线性关系
//第一次重构：重新设计unitblock，内存池构造为：128个大小为512的内存块
    //如果需求空间大于1024，就用外部申请了
    //又经过测试，delete所用时间要比new所用时间多得多
    //唯一问题具体描述：二维数组在resize时，出现问题

#include "lab7.h"
#include <time.h>
#define boxsize 128
#define blocksize 500
clock_t start, stop;
double duration;

template<class T>
using MyAllocator = alloc_mem_pool<T>; // replace the std::allocator with your allocator
using Point2D = std::pair<int, int>;

const int TestSize = 30000;
const int PickSize = 5000;

template <class T>
alloc_mem_pool<T>::alloc_mem_pool()//建立内存池
{
    freelist = NULL;
    realaddress = NULL;
    realend = NULL;
    boxindex = 0;
    extraindex = 0;
}



template <class T>
alloc_mem_pool<T>::~alloc_mem_pool()
{
    int i = 0;
    for (i = 0; i < boxindex; i++)
        delete []boxlist[i];
    // for (i = 0; i < extraindex; i++)
    //     delete []extralist[i];
}

template <class T>
[[nodiscard]] T* alloc_mem_pool<T>::allocate(size_t n)//分配以该指针为开始，连续n个内存空间（以T的大小确定空间大小）
{
    T* result = new T[n];
    return result;
    //if (n > blocksize)
    // if (1)
    // {
    //     T* result = new T[n];
    //     cout << "no" << endl;
    //     if (result != NULL)
    //     {
    //         extralist[extraindex] = result;
    //         extraindex++;
    //     }
    //     return result;
    // }
    // else if (freelist != NULL)
    // {
    //     unitblock* temp = freelist;
    //     freelist = freelist->next;
    //     cout << "yes" << endl;
    //     return temp->element;
    // }
    // else
    // {
    //     int oldindex = boxindex;
    //     boxlist[boxindex] = new unitblock[boxsize];
    //     boxindex++;
    //     freelist = boxlist[oldindex];
    //     int i = 0;
    //     for (i = 1; i < boxsize; i++)
    //     {
    //         freelist->next = boxlist[oldindex] + i;
    //         freelist = freelist->next;
    //     }
    //     freelist->next = NULL;
    //     freelist = boxlist[oldindex];
    //     unitblock* temp = freelist;
    //     freelist = freelist->next;
    //     cout << "nop" << endl;
    //     return temp->element;
    // }
}

template <class T>
void alloc_mem_pool<T>::deallocate(pointer p, size_t n)//正常这里应当释放空间，但此处将空间暂存于freelist中
{
    delete[] p;
    //delete[] p;
    // if (p != NULL)//recycle the space: put "p" to the first place of freelist
    // {
    //     reinterpret_cast<unitblock*>(p)->next = freelist;//这里，因为本身每个空间后就有预留的next位置，因此不会影响
    //     freelist = reinterpret_cast<unitblock*>(p);
    // }
}




void standardfunc()
{
    std::random_device rd;//生成随机数种子
    std::mt19937 gen(rd());//生成随机数
    std::uniform_int_distribution<> dis(1, TestSize);//再次随机

    // vector creation
    using IntVec = std::vector<int>;//第一个测试，以int二维数组为例
    std::vector<IntVec> vecints(TestSize);//vecints为int二维数组
    for (int i = 0; i < TestSize; i++)
        vecints[i].resize(dis(gen));//对其中的每一个向量进行重新的内存分配
    using PointVec = std::vector<Point2D>;//第二个测试，以PointVec类的二维数组为例
    std::vector<PointVec> vecpts(TestSize);//vecpts为类的二维数组
    for (int i = 0; i < TestSize; i++)
        vecpts[i].resize(dis(gen));//对其中的每一个向量进行重新的内存分配
    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints[idx].resize(size);
        vecpts[idx].resize(size);
    }

    // vector element assignment
    {
        int val = 10;
        int idx1 = dis(gen) - 1;
        int idx2 = vecints[idx1].size() / 2;
        vecints[idx1][idx2] = val;
        if (vecints[idx1][idx2] == val)
        std::cout << "correct assignment in vecints: " << idx1 << std::endl;
        else
        std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    }
    {
        Point2D val(11, 15);
        int idx1 = dis(gen) - 1;
        int idx2 = vecpts[idx1].size() / 2;
        vecpts[idx1][idx2] = val;
        if (vecpts[idx1][idx2] == val)
        std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
        std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }

    return;
}

void allocmempoolfunc()
{
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> dis(1, TestSize);

    // vector creation
    using IntVec = std::vector<int, MyAllocator<int>>;
    std::vector<IntVec, MyAllocator<IntVec>> vecints(TestSize);
    for (int i = 0; i < TestSize; i++)
       vecints[i].resize(dis(gen));
    using PointVec = std::vector<Point2D, MyAllocator<Point2D>>;
    std::vector<PointVec, MyAllocator<PointVec>> vecpts(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts[i].resize(dis(gen));
    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints[idx].resize(size);
        vecpts[idx].resize(size);
    }

    // vector element assignment
    {
        int val = 10;
        int idx1 = dis(gen) - 1;
        int idx2 = vecints[idx1].size() / 2;
        vecints[idx1][idx2] = val;
        if (vecints[idx1][idx2] == val)
        std::cout << "correct assignment in vecints: " << idx1 << std::endl;
        else
        std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    }
    {
        Point2D val(11, 15);
        int idx1 = dis(gen) - 1;
        int idx2 = vecpts[idx1].size() / 2;
        vecpts[idx1][idx2] = val;
        if (vecpts[idx1][idx2] == val)
        std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
        std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }

    return;
}

int main()
{
    start = clock();
    standardfunc();
    stop = clock();
    duration = ((double)(stop - start)) / CLK_TCK;
    cout << "the standard duration: " << duration << endl;
    start = clock();
    allocmempoolfunc();
    // int* testa[100];
    // int* tp;
    // for (int i = 0; i < 100; i++)
    // {
    //     tp = new int[5];
    //     testa[i] = tp;
    // }
    // for (int i = 0; i < 100; i++)
    // {
    //     delete []testa[i];
    // }
    stop = clock();
    duration = ((double)(stop - start)) / CLK_TCK;
    cout << "the alloc mem pool duration: " << duration << endl;

}