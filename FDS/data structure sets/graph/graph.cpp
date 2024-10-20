//注意：图和堆的存储相似，在0点不存数据，graph[1][2]指的就是1-2间的距离，遍历时需要注意
//注意：本程序用的都是上面的方法，0点不存数据
//这是一个关于图的各种算法的总代码，实现的函数如下：
//0. initialize()/buildgraph(): 用邻接矩阵的方式构成图
//1. DFS
    //a. DFS_recursion(int i): DFS的递归算法
    //但递归算法最为抽象的是难以封装，所以在这个项目中必须通过如下代码进行使用：
    // for (int i = 1; i <= onegraph.v; i++)
    // DFS_recursion(i);
    //b. DFS_nrecursion(): DFS的非递归算法
//2. topsort()：根据图直接输出拓扑排序的结果（结尾有空格）
//注意：
    //1. 拓扑排序时图中不能有环，否则会忽略环中的所有点
    //2. 图中必须存在入度为0的点作为起始点，否则无输出
//3. dijkstra(): 输入起始点和结束点，根据图输出最短距离、最短路径
//4. networkflow(): 输入起始点和结束点，根据图中边的权值输出最大流量
    //此函数需要用到子函数: networkflow_DFS()
//5. prim(): 根据图输出最小生成树的总距离，以及一个最小生成树的例子
//6. kruskal(): 根据图输出最小生成树的总距离，以及一个最小生成树的例子
    //此函数需要用到子函数: kruskal_connect(int x, int y, int* data); kruskal_find(int x, int* data)



//记录：在定义新的图时出现问题，经排查后发现是内存超限了
//因此要有一个概念：1000x1000的数组是很大的，定义了两个就已经不太行了(x


//TODO:
//1. 最小生成树判定是否唯一，有一些问题，先注释掉了相关的代码

#include <bits/stdc++.h>
#define MAX_N 100
#define INF 10000
using namespace std;

struct Graph
{
    int data[MAX_N][MAX_N];//数据
    int e;//边
    int v;//顶点
};
Graph onegraph; //图的主体

//函数：初始化图
void initialize()
{
    int i, j;
    for (i = 0; i < MAX_N; i++)
    {
        for (j = 0; j < MAX_N; j++)
            onegraph.data[i][j] = -1;
    }
}

//函数：构造图
void buildgraph()
{
    int state = 0;
    cout << "please choose the form of graph: undirected(1), directed(0)" << endl;
    cin >> state;
    cout << "please input the number of vertices and edges:" << endl;
    cin >> onegraph.v >> onegraph.e;
    cout << "please input start vertice/end vertice/distance:" << endl;
    int i = 0, v1, v2, tempdist;
    for (i = 0; i < onegraph.e; i++)
    {
        cin >> v1 >> v2 >> tempdist;
        onegraph.data[v1][v2] = tempdist;
        if (state == 1)
            onegraph.data[v2][v1] = tempdist;
    }
}

//函数：深度优先遍历（DFS）（递归/非递归）
//原理：
    //1. 以某个点为起始点
    //2. 输出该点
    //3. 对该点邻近的所有点作DFS
void DFS_recursion(int start)
{
    static int visited[MAX_N];
    if (visited[start] == 1)
        return;
        cout << start << " ";
    visited[start] = 1;
    for (int i = 1; i <= onegraph.v; i++)
    {
        if (onegraph.data[start][i] > 0 && visited[i] != 1)
            DFS_recursion(i);
    }
}

void DFS_nrecursion()
{
    int* tempstack = new int[2 * onegraph.v];
    int* visited = new int[onegraph.v + 1];
    for (int i = 0; i <= onegraph.v; i++)
        visited[i] = -1;
    int stackpt = 0, target;
    cout << "the DFS result is: ";

    for (int i = 1; i <= onegraph.v; i++)
    {
        //将这个点写入栈
        if (visited[i] == -1)
        {
            *(tempstack + stackpt) = i;
            stackpt++;
        }
        while (stackpt != 0)
        {
            stackpt--;
            target = *(tempstack + stackpt);
            cout << target << " ";
            visited[target] = 1;
            //将所有邻近的点写入栈
            for (int j = 1; j <= onegraph.v; j++)
            {
                if (visited[j] == -1 && onegraph.data[target][j] > 0)
                {
                    *(tempstack + stackpt) = j;
                    stackpt++;
                }
            }
        }
    }
}

//函数：拓扑排序
//原理：
    //1. 计算所有点入度(indegree)
    //2. 把所有入度为0的点入队列
    //3. 逐个输出入度为0的点，并计算其他可能会入度变化的点
    //4. 如果有其他点入度变为0，那么入队列
    //5. 直至队列中所有点抛出为止
void topsort()
{
    //step 1: 计算所有点的入度，存到数组中
    int* indegree = new int[onegraph.v];//记录入度的数组
    int simqueue[MAX_N];//暂存队列
    int ps = 0;//队列初始指针
    int pr = 0;//队列结束指针
    int i, j, counter, target;
    for (i = 1; i <= onegraph.v; i++)
    {
        counter = 0;
        for (j = 1; j <= onegraph.v; j++)
        {
            if (onegraph.data[j][i] > 0)
                counter++;
        }
        indegree[i] = counter;
    }

    //step 2: 把所有入度为0的数据入队
    for (i = 1; i <= onegraph.v; i++)
    {
        if (indegree[i] == 0)
        {
            *(simqueue + pr) = i;
            pr++;
        }
    }

    //step 3: 依次出队，对于每个点，其作为入度的点对应出度的点入度-1.
    cout << "the topsort of the graph is: ";
    for (; ps < pr; i++)
    {
        target = *(simqueue + ps);
        cout << target << " ";
        ps++;
        for (i = 1; i <= onegraph.v; i++)
        {
            if (onegraph.data[target][i] > 0)//target-i有路径
            {
                if ((--indegree[i]) == 0)
                {
                    *(simqueue + pr) = i;
                    pr++;
                }
            }
        }
    }
    //delete indegree;
    cout << endl;
}

//函数：最短路径——dijkstra算法
//原理：
    //0. 由三个数组作为总的调度：
        //a. 记录起始点到这个点的最短距离
        //b. 记录点是否被访问过
        //c. 记录点的父节点
        //此代码中，分别对应statelist[0/1/2]; -1表示未访问，1表示已访问
    //1. 从起始点开始：
        //a. 对该点的临近点更新距离（即起始点到该点距离+该点到临近点距离>起始点到临近点距离，那么更新）
        //在更新时所做的事情：1. 距离更新 2. 父节点更新（更新为该点）
        //b. 记录该点为已访问
    //2. 选取现在距离最短的未访问的点，重新做1
    //3. 直至完全遍历为止
    //4. 从结束点开始，通过父节点反向寻找整条路径（可以用栈的方式）
//易错：在初始化状态时：起始点的距离为0，而不是INF！
void dijkstra()
{
    int start, end, target;
    cout << "please input the start vertice and the end vertice: " << endl;
    cin >> start >> end;
    if (start < 0 || end < 0 || start > onegraph.v || end > onegraph.v)
    {
        cout << "the input is out of range." << endl;
        return;
    }
    //step 0: 预设
    // int* statelist[3];//主要数组
    // for (int q = 0; q < 3; q++)
    //     statelist[q] = new int[onegraph.v + 1];
    int statelist[3][1000];
    int i, j, min, save;
    for (i = 1; i <= onegraph.v; i++)
    {
        statelist[0][i] = INF;
        statelist[1][i] = statelist[2][i] = -1;
    }
    statelist[0][start] = 0;
    //step 1: 循环
    while (1)
    {
        //step a: 寻找target
        if (statelist[1][start] == -1)
            target = start;
        else
        {
            target = -1;
            for (i = 0, min = INF; i <= onegraph.v; i++)
            {
                if (statelist[1][i] == -1 && statelist[0][i] != INF && statelist[0][i] < min)
                {
                    target = i;//如果是未访问/可延伸到/路径最短的点，设为target
                    min = statelist[0][i];
                }
            }
            if (target == -1)
                break;
        }

        //step b：开始更新
        for (i = 1; i <= onegraph.v; i++)
        {
            if (onegraph.data[target][i] > 0)
            {
                if (onegraph.data[target][i] + statelist[0][target] < statelist[0][i])
                {
                    statelist[0][i] = onegraph.data[target][i] + statelist[0][target];
                    statelist[2][i] = target;
                }
            }
        }

        //step c: 记录该点已访问
        statelist[1][target] = 1;
    }

    //step 2: 输出结果
    if (statelist[0][end] == INF)
    {
        cout << "there is no path from " << start << " to " << end << endl;
        return;
    }
    int* result = new int[onegraph.v + 1];
    int stackp = 0;
    cout << "the shortest distance is: " << statelist[0][end] << endl;
    cout << "the shortest path is: ";
    for (i = end;i != start; i = statelist[2][i])
    {
        *(result + stackp) = statelist[2][i];
        stackp++;
    }
    while (stackp != 0)
    {
        stackp--;
        cout << *(result + stackp) << " ";
    }
    cout << end;
}

//函数；网络流问题
//目的：每一条边都有一定的容量，从起点到终点进行流量传输，求能够到达终点的最大流量
//可以类比为水管通道，就是从起点到终点最多能通过多少水
//原理：
    //1. 用DFS/BFS的方式找到一条从起点到终点的路径
    //2. 取路径边中最小的容量作为容量，计入最大流中
    //3. 加入反向弧（就是该路径上的所有边反向，具体为：正向的减去流量，反向的加上流量）
    //4. 重复1-3，直至找不到从起点到终点的路径
    //5. 输出结果
//解释：对于3我们可以这样理解：
    //加入反向弧后，程序有反悔的机会，但要注意的是：用到反向弧的路径，它所表征的并不是实际的流动！
    //实际上，利用反向弧时，在起点它已经与设置反向弧的那个流动路径“交换”了流量！
    //换言之，之前路径利用这部分弧通过的流量被现在的路径“代理”了，而现在路径中利用反向弧之后的部分
    //实际上是之前路径“反悔”的部分。
    //弄明白这一点就知道反向弧到底是如何工作的了。
//易错：本程序为了尽可能实现封装，因此尝试在递归的基础上还能进一步不设置外部的动态变量
    //但这会大大增加整个题目的复杂度
    //问题主要在于：对于static变量，它只能在函数内被初始化一次，那么如何多次整体地调用递归函数呢？
    //因为在用了一次之后，static变量已经不是初值了，而为了保证递归，又不能直接在函数内赋值
    //因此需要各种各样的曲线救国的办法来实现
        //1. 在中止递归的时候，初始化static，并且设置endsign，其余堆栈内所有程序直接跳出
        //2. 对于endsign本身，在之后再次使用递归函数的时候，利用外部条件特判
    //但依旧很窝囊，只能说唯一的好处就是把整个方法封装起来，移植的时候不会有任何影响了TAT

void networkflow_DFS(int start, int end, int* path, Graph tempgraph)
{
    //递归方式求取从start到end的一条路径，路径末尾为path
    static int pathpt = 0;//这是path的位置
    static int* visited = new int[tempgraph.v + 1];//这是标记已访问的数组
    static bool endsign = false;//这是中止递归的标识

    //函数内限制：传入的path中，path[0]必须为-1（但被封装在函数内解决了，不会影响使用）
    if (path[0] == -1)
        endsign = false;//更新endsign

    //第一处调停：避免visited数据被更改
    if (endsign)
        return;

    //处理当前的数据
    visited[start] = 1;
    path[pathpt] = start;
    pathpt++;

    //到达末尾，中止所有递归
    if (start == end)
    {
        endsign = true;//开启中止递归
        path[pathpt] = -1;//结束path的录入
        for (int i = 0; i <= tempgraph.v; i++)//初始化visited
            visited[i] = -1;
        pathpt = 0;//初始化pathpt
        return;
    }
    else endsign = false;

    //正常寻找所有邻近点并递归
    for (int i = 1; i <= tempgraph.v; i++)
    {
        if (tempgraph.data[start][i] > 0 && visited[i] != 1)
            networkflow_DFS(i, end, path, tempgraph);
    }

    //第二次调停：避免pathpt被更改
    if (endsign)
        return;
    
    //退栈，这个path没有路径可走了
    pathpt--;

    //找到最后一个递归还没有路径，那么就是无路径了
    if (pathpt == 0)
        path[0] = -1;
}

void networkflow()
{
    //网络流需要更改图中的权重，因此选择备份一个图
    Graph tempgraph = onegraph;
    int* path = new int[onegraph.v + 1];
    int pathpt = 1, start, end, target, min, result = 0;
    cout << "please input the start vertice and end vertice:" << endl;
    cin >> start >> end;
    if (start == end)
    {
        cout << "the flow amount is 0." << endl;
        return;
    }
    path[0] = start;

    //开始循环
    while (1)
    {
        min = INF;

        //step 1: 找到一条从start到end的路径
        networkflow_DFS(start, end, path, tempgraph);

        //没有路径，则直接跳出循环
        if (path[0] == -1)
            break;

        //step 2: 确定这条路径中的最小权重
        for (int i = 0; path[i + 1] != -1; i++)
        {
            if (tempgraph.data[path[i]][path[i + 1]] < min)
                min = tempgraph.data[path[i]][path[i + 1]];
        }

        //step 3: 利用最小权重，更改图中的权重并设置反向弧
        for (int i = 0; path[i + 1] != -1; i++)
        {
            tempgraph.data[path[i]][path[i + 1]] -= min;
            if (tempgraph.data[path[i + 1]][path[i]] == -1)
                tempgraph.data[path[i + 1]][path[i]] = min;
            else tempgraph.data[path[i + 1]][path[i]] += min;
        }

        //step 4: 记录数据并协助递归函数初始化path
        result += min;
        path[0] = -1;
    }

    //step 5: 输出结果
    cout << "the flow amount is " << result << endl;
}

//函数：prim's algorithm（用于寻找最小生成树）
//原理：
    //1. 从某一点开始：更新与其邻近的所有点的权值
    //2. 寻找临近点中权值最小的点
    //3. 将点和这个边计入集合中
    //4. 重复1-3直至所有点被访问
    //（这与dijkstra算法很相像）
//注意：实际上最小生成树中，每个点是否选取其实只与它邻近的点有关系
//这也是为什么实际上从任何一个点出发，最终的路程都是一样的原因
void prim()
{
    int target = 1, min = INF, nexttarget = 0, index = 0;
    int* weight = new int[onegraph.v + 1];
    int* visited = new int[onegraph.v + 1];
    int* father = new int[onegraph.v + 1];
    int resultpair[2][1000];
    int result = 0;
    bool endsign, isunique = true;
    for (int i = 0; i <= onegraph.v; i++)
        visited[i] = -1, weight[i] = INF;
    weight[target] = INF;

    //开始循环
    while (1)
    {
        //初始化min，并标记target
        min = INF;
        visited[target] = 1;

        //对target的所有临近点的权值进行更新
        for (int i = 1; i <= onegraph.v; i++)
        {
            if (onegraph.data[target][i] > 0 && visited[i] == -1)
            {
                if (onegraph.data[target][i] < weight[i])
                {
                    weight[i] = onegraph.data[target][i];
                    father[i] = target;
                }
            }
        }
        
        //选取现在邻近点中权值最小的作为下一个target（weight为有限值则为邻近点）
        for (int i = 1; i <= onegraph.v; i++)
        {
            if (weight[i] < min && visited[i] == -1)
            {
                min = weight[i];
                nexttarget = i;
            }
            // else if (weight[i] == min && weight[i] != INF)
            //     isunique = false;
        }

        //没有临近点了，要么是连完了要么这不是一个连通图
        if (nexttarget == 0)
        {
            endsign = true;
            for (int i = 1; i <= onegraph.v; i++)
                if (visited[i] == -1)
                    endsign = false;
            if (endsign)
                break;
            else
            {
                cout << "there is no spanning tree for this graph." << endl;
                return;
            }
        }

        //记录结果（由于选取的边不一定就是target邻近的边，因此边的father也需要记录）
        resultpair[0][index] = father[nexttarget];
        resultpair[1][index] = nexttarget;
        index++;
        target = nexttarget;

        //重置nexttarget
        nexttarget = 0;
        result += min;
    }

    //输出结果
    // if (isunique)
    //     cout << "the spanning tree is unique." << endl;
    // else cout << "the spanning tree is not unique." << endl;
    cout << "the minimum distance is: " << result << endl;
    cout << "the edges are: " << endl;
    for (int i = 0; i < index; i++)
    {
        cout << resultpair[0][i] << "-" << resultpair[1][i] << " ";
        if (i % 4 == 3)
            cout << endl;
    }
}

//函数：kruskal's algorithm（用于寻找最小生成树）
//原理：
    //1. 从所有边中选取权值最小的边
    //2. 判断它与已经选的边是否构成环（利用并查集，看边的两点是否关联）
    //3. 未构成环则选取，否则不选取
    //4. 重复1、2、3直至T取了n-1条边（结束）或所有边都被取完（没有最小生成树）
//注意：由于这个方法利用的是边的特性，因此要重构读图的方式，并且需要建立并查集
//注释：这个方法最好用C++的vector写，这里单纯用C写，对删除等操作会很难受
struct edgeset
{
    int start;
    int end;
    int dist;
};

//两个派生的并查集函数

int kruskal_find(int x, int* data)
{
    if (data[x] < 0)
        return x;
    else return (data[x] = kruskal_find(data[x], data));
}

void kruskal_connect(int x, int y, int* data)
{
    int kx = kruskal_find(x, data);
    int ky = kruskal_find(y, data);
    if (kx != ky)
    {
        if (data[kx] < data[ky])
        {
            data[kx] += data[ky];
            data[ky] = kx;
        }
        else
        {
            data[ky] += data[kx];
            data[kx] = ky;
        }
    }
}

void kruskal()
{
    //初始化：对总共的所有边进行初始化
    edgeset* selected = new edgeset[onegraph.v + 1];
    edgeset* total = new edgeset[onegraph.v + 1];
    int indext = 0, indexs = 0, min = INF, target;
    int tempx, tempy, tempdist;
    int result = 0;
    int* tempunion = new int[onegraph.v + 1];
    for (int i = 1; i <= onegraph.v; i++)
    {
        for (int j = i; j <= onegraph.v; j++)
        {
            if (onegraph.data[i][j] > 0)
            {
                total[indext].start = i;
                total[indext].end = j;
                total[indext].dist = onegraph.data[i][j];
                indext++;
            }
        }
    }
    //初始化并查集
    for (int i = 0; i <= onegraph.v; i++)
        tempunion[i] = -1;

    //提前的准备：因为我们取的一直都是最小的边，不如直接将它从大到小排列
    edgeset temp;
    for (int i = 0; i < indext; i++)
    {
        temp = total[i];
        for (int j = i; j > 0 && total[j - 1].dist < temp.dist; j--)
        {
            total[j] = total[j - 1];
            total[j - 1] = temp;
        }
    }

    //开始循环  
    while (1)
    {
        min = INF;
        if (indexs == onegraph.e || indext == 0)
            break;
        
        //记录这条边的数据
        indext--;
        temp = total[indext];
        tempx = total[indext].start;
        tempy = total[indext].end;
        tempdist = total[indext].dist;

        //查询是否为环
        if (kruskal_find(tempx, tempunion) == kruskal_find(tempy, tempunion))
            continue;
        else 
        {

            //不为环，加入selected数组并记录距离
            kruskal_connect(tempx, tempy, tempunion);
            result += tempdist;
            selected[indexs] = temp;
            indexs++;
        }
    }

    //如果边数足够则有树，否则无树
    if (indext == 0 && indexs != onegraph.v - 1)
        cout << "there is no spanning tree for this graph." << endl;
    else 
    {
        cout << "the minimum distance is: " << result << endl;
        cout << "the edges are: " << endl;
        for (int i = 0; i < indexs; i++)
        {
            cout << selected[i].start << "-" << selected[i].end << " ";
            if (i % 4 == 3)
                cout << endl;
        }
    }
}

int main()
{
    initialize();
    buildgraph();
    //prim();
    //kruskal();
    //topsort();
    //networkflow();
    //topsort();
    //dijkstra();
    //DFS_nrecursion();
}