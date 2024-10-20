//并查集的功能实现
//并查集以一维数组的方式存储,其中的值为正数代表索引的根，为负数代表该索引为根，以它为根的树的节点的相反数
//关键函数：find_root，考虑是否有路径压缩
//在这个程序中，结点从0开始
#include <bits/stdc++.h>
#define MAX_N 10
using namespace std;

int oneunion[MAX_N];//a union for some vertices

int find_root(int v)//key function:find
{
    if (oneunion[v] < 0)//find the root
        return v;
    else return (oneunion[v] = find_root(oneunion[v]));//with path compassion
}

void initialize()//at first, set all the value in the union as -1.
{
    for (int i = 0; i < MAX_N; i++)
        oneunion[i] = -1;
}

void connect()//connect v1 and v2
{
    int v1, v2, root1, root2;
    cin >> v1 >> v2;
    root1 = find_root(v1);
    root2 = find_root(v2);
    if (root1 != root2)//v1 and v2 havn't connected
    {
        if (oneunion[root1] < oneunion[root2])
        {
            oneunion[root1] += oneunion[root2];//do the succession
            oneunion[root2] = root1;//change the root with less branches
        }
        else 
        {
            oneunion[root2] += oneunion[root1];
            oneunion[root1] = root2;
        }
    }
}

void check(int v1, int v2)
{
    if (find_root(v1) == find_root(v2))
        cout << v1 << " and " << v2 << "is connected" << endl;
    else cout << v1 << " and " << v2 << "is not connected" << endl;
}

int main()
{
    initialize();
    connect();
    connect();
    connect();
    check(1, 4);
}