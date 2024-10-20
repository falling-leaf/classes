//关于排序
//冒泡排序：O(n^2)
    //两次遍历，对每一对相邻元素进行排序
    //稳定性
//选择排序：O(n^2)
    //两次遍历：对每一个元素，依次向前挪动直至它在前面正确的位置
    //对几乎已经排好的序列最快
    //1 4 5 6 2 3
    //1 4 5 2 6 3
    //1 4 2 5 6 3
    //1 2 4 5 6 3...
//注：bubble 和insertion均为相邻元素的排序，其复杂度必大于O(n^2)
//希尔排序：O(n^(7/6))
    //每隔increment个数取一个数，来执行insertion sort
    //increment的值，从总数n/2开始，直到1
        //increment的取法：自除2，或者取2^k - 1 
//堆排序：O(nlogn)
    //建立大根堆->做删除最大值的操作（让最大值排在最后面）
//快速排序：O(nlogn)
    //当N<20时，quicksort要比insertionsort慢
    //step1: 选取随机的一个点（用median）
    //step2: 设头尾两个指针，大于该数的排在尾部，小于该数的排在头部
    //step3：递归，分治

#include <bits/stdc++.h>
#include <time.h>
#define MAX_N 30000
using namespace std;
int data[MAX_N];
clock_t start, stop;
double duration;

void initialize()
{
    srand(time(NULL));
    for (int i = 0; i < MAX_N; i++)
        data[i] = rand() % 10000;
}

void oneswap(int* a, int* b)
{
    int temp = *a;
    int temp1 = *b;
    *a = temp1;
    *b = temp;
    return;
}

void bubblesort()
{
    int temp;
    start = clock();
    for (int i = 0; i < MAX_N; i++)
    {
        for (int j = MAX_N - 1; j > i; j--)
        {
            if (data[j - 1] > data[j])
            {
                temp = data[j - 1];
                data[j - 1] = data[j];
                data[j] = temp;
            }
        }
    }
    stop = clock();
    duration = ((double)(stop - start)) / CLK_TCK;
    cout << "the time of bubble sort is " << duration << endl;
    cout << "part of the result is: ";
    for (int i = 0; i < 10; i++)
        cout << data[i] << " ";
    cout << endl;
}

void insertionsort()
{
    int temp;
    start = clock();
    for (int i = 0; i < MAX_N; i++)
    {
        temp = data[i];
        for (int j = i; j > 0 && data[j - 1] > temp; j--)
        {
            data[j] = data[j - 1];
            data[j - 1] = temp;
        }
    }
    stop = clock();
    duration = ((double)(stop - start)) / CLK_TCK;
    cout << "the time of insertion sort is " << duration << endl;
    cout << "part of the result is: ";
    for (int i = 0; i < 10; i++)
        cout << data[i] << " ";
    cout << endl;
}

void shellsort()
{
    int temp, increment, j;
    start = clock();
    for (increment = MAX_N / 2; increment > 0; increment /= 2)
    {
        for (int i = increment; i < MAX_N; i++)
        {
            temp = data[i];
            for (j = i; j >= increment; j -= increment)
            {
                if (data[j - increment] > temp)
                    data[j] = data[j - increment];
                else break;
            }
            data[j] = temp;
        }
    }
    stop = clock();
    duration = ((double)(stop - start)) / CLK_TCK;
    cout << "the time of shell sort is " << duration << endl;
    cout << "part of the result is: ";
    for (int i = 0; i < 10; i++)
        cout << data[i] << " ";
    cout << endl;
}

void percolatedown(int p, int size)
{
    int temp, tdata;
    if (2 * p + 1 > size - 1)
        return;
    if (2 * p + 2  <= size - 1)
    {
        if (data[2 * p + 1] > data[2 * p + 2])
            temp = 2 * p + 1;
        else temp = 2 * p + 2;
    }
    else temp = 2 * p + 1;

    if (data[p] < data[temp])
    {
        tdata = data[p];
        data[p] = data[temp];
        data[temp] = tdata;
        percolatedown(temp, size);
    }
    else return;
}

void heapsort()
{
    start = clock();
    int i, temp, size;
    for (i = MAX_N / 2; i >= 0; i--)
        percolatedown(i, MAX_N);
    size = MAX_N;
    while (size > 0)
    {
        temp = data[0];
        data[0] = data[size - 1];
        data[size - 1] = temp;
        size--;
        percolatedown(0, size);
    }
    stop = clock();
    duration = ((double)(stop - start)) / CLK_TCK;
    cout << "the time of heap sort is " << duration << endl;
    cout << "part of the result is: ";
    for (int i = 0; i < 10; i++)
        cout << data[i] << " ";
    cout << endl;
}

int median(int left, int right)
{
    int center = (left + right) / 2;
    if (data[left] > data[center])
        oneswap(&data[left], &data[center]);
    if (data[left] > data[right])
        oneswap(&data[left], &data[right]);
    if (data[center] > data[right])
        oneswap(&data[center], &data[right]);
        //now, left <= center <= right
    oneswap(&data[center], &data[right - 1]);
    //the pivot is center, now is right - 1
    return data[right - 1];
}

void quicksort(int left, int right)
{
    if (left >= right)
        return;
    int pivot, i, j;
    //if (left + cutoff <= right)
    //{
        pivot = median(left, right);
        i = left;
        j = right - 1;
        while (1)
        {
            while (data[++i] < pivot){}
            while (data[--j] > pivot){}
            if (i < j)
                oneswap(&data[i], &data[j]);
            else break;
        }
        oneswap(&data[i], &data[right - 1]);
        quicksort(left, i - 1);
        quicksort(i + 1, right);
    //}
    //else insertionsort();
}

int main()
{
    initialize();
    bubblesort();
    initialize();
    insertionsort();
    initialize();
    shellsort();
    initialize();
    heapsort();
    initialize();
    start = clock();
    quicksort(0, MAX_N - 1);
    stop = clock();
    duration = ((double)(stop - start)) / CLK_TCK;
    cout << "the time of quick sort is " << duration << endl;
    cout << "part of the result is: ";
    for (int i = 0; i < 10; i++)
        cout << data[i] << " ";
    cout << endl;
}