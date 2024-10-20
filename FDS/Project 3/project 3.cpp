#include "project 3.h"
#include <time.h>
#define inf -1  //set the infinity as -1, means the path is not in consideration

int onestack[10000];//the stack to cope with the circle
int visited[10000];//record the visit state of DFS
int circlepath[1000][1000];//record the path of circle
int circledist[1000];//record the distance of every circle
int stackpt = 0;//stack pointer
int circlept = 0;//circle pointer

void initialize()//set all the edges as -1 at first.
{
    int i, j;
    for (i = 0; i < 1100; i++)//traversal
    {
        for (j = 0; j < 1100; j++)
            map_path.data[i][j] = -1;//set all the path unaccessible
    }
    for (i = 0; i < 10000; i++)
    {
        visited[i] = 0;
    }
}

void buildgraph()//build graph as the input by the user
{
    int v, e, i, va, vb, d;
    cout << "please input the number of vertices and edges." << endl;
    cin >> v >> e;
    map_path.vertice = v;//set the vertice of the graph
    map_path.edge = e;//set the edge of the graph
    for (i = 0; i < e; i++)//set the data of the map
    {
        cin >> va >> vb >> d;
        map_path.data[va][vb] = d;
    }
}

void buildrandom()//build graph at random
{
    int v, e, i, j, va, vb, d;
    char judge;
    bool sign = true;
    bool read = false;
    cout << "please input the vertice you want to set." << endl;//input the number of vertices
    cin >> v;
    map_path.vertice = v;
    srand(time(NULL));//set the random seed
    cout << "the data of the graph is as follows." << endl;//output the data of the graph
    for (i = 1; i < v; i++)
    {
        for (j = i + 1; j <= v; j++)
        {
            d = rand() % 100;//set a random distance
            if (d <= 50)//set a high judge level to get a dense graph
            {
                if (d == 0)
                    d++;
                d *= 20;//extend the random digit
                map_path.data[i][j] = d;//set the data
                if (i == 1 && j == v)
                    map_path.data[i][j] = -1;//set the start and end no access
                if (sign && v >= 50)//the v is too large 
                {
                    cout << "the vertice is too large, do you still want to see the detail of the graph?" << endl;
                    cout << "input 'y': yes; 'n': no" << endl;
                    cin >> judge;
                    if (judge == 'y')//still see the detail
                        read = true;
                    else read = false;
                    sign = false;
                }
                if (read || v < 50)//output the weight of every edge
                    cout << "the distance of " << i << " and " << j << " is " << d << endl;
            }
        }
    }
}

void searchshortest(int start, int end, int* odis, int* opath)
{
    int pathlist[3][map_path.vertice + 1];
    //first row save the distance
    //second row save the father path
    //third row save having visited or not
    int i = 0, j = 0, min = 0, v = end;
    int check = map_path.vertice + 99;//set the result index
    for (i = 1; i <= map_path.vertice; i++)//start initializing
    {
        if (map_path.data[start][i] >= 0)//the start can reach the vertice
        {
            pathlist[0][i] = map_path.data[start][i];//set the pathlist
            pathlist[1][i] = start;
        }
        else 
        {
            pathlist[0][i] = 0;//set the pathlist as 0
            pathlist[1][i] = 0;
        }
        pathlist[2][i] = 0;//the state of vertice set to be "unvisited"
    }
    pathlist[2][1] = 1;//the state of vertice 1 set to be "visited"
    while (1)
    {
        min = 0;//searth the min path
        for (i = 1; i <= map_path.vertice; i++)
        {
            if (pathlist[2][i] == 0 && pathlist[0][i] != 0)//find the path with the min distance
            {
                if (min == 0)
                    min = i;
                else if (pathlist[0][i] < pathlist[0][min])
                    min = i;
            }
        }
        if (min == 0)
            break;
        for (i = 1; i <= map_path.vertice; i++)//begin to set this vertice
        {
            if (map_path.data[min][i] >= 0)//reset all the vertice according to the "min" vertice
            {
                if (pathlist[0][i] == 0 || pathlist[0][min] + map_path.data[min][i] < pathlist[0][i])
                {
                    pathlist[0][i] = pathlist[0][min] + map_path.data[min][i];
                    pathlist[1][i] = min;
                }
            }
        }
        pathlist[2][min] = 1;//set the vertice as "visited"
    }
    *odis = pathlist[0][end];//get the out distance
    if (*odis == 0)//there is no path
    {
        opath[0] = 0;
        return;
    }
    opath[check] = end;
    check--;
    while (pathlist[1][v] != start)//reverse the path data
    {
        opath[check] = pathlist[1][v];//set the opath
        v = pathlist[1][v];//seek the root
        check--;
    }
    opath[check] = start;
    min = check;
    for (;check <= map_path.vertice + 99; check++)//turn the data into the start of the array
        opath[check - min] = opath[check];
    opath[check - min] = 0;
}


void DFS(int p, int start)//the assigned DFS
{
    int i = 0;
    visited[p] = 1;//set the state of visit
    onestack[stackpt] = p;//record the stack data
    stackpt++;//update the stack pointer
    if (p == start && stackpt > 1)//it forms a circle
    {
        int sum = 0;
        circlepath[circlept][0] = p;//begin to calculate the distance
        for (i = 1; i < stackpt; i++)//the distance traversal
            sum += map_path.data[onestack[i - 1]][onestack[i]], circlepath[circlept][i] = onestack[i];
        circledist[circlept] = sum;
        circlept++;
        stackpt--;//pop the stack, to find another circle
        return;
    }
    for (i = 0; i <= map_path.vertice; i++)
    {
        //if (map_path.data[p][i] > 0 && ((visited[i] == 0) || (i == start)))
        if (map_path.data[p][i] > 0 && ((visited[i] == 0) || (i == start)))
            DFS(i, start);//do the DFS
    }
    stackpt--;
    return;
}


//search the second shortest path
void search2nd(int start, int end, int* ipath, int idis, int* odis, int* opath)
{
    int i = 0, j = 0, k = 0;
    int cuts, cute;
    int save;
    *odis = 0;
    int tempdis;
    int targetv;
    int temppath[10000];
    //int* temppath = (int*)malloc(sizeof(int)*(map_path.vertice + 100));//save the temp path
    for (j = 1; ipath[j] != 0; i++, j++)
    {
        cuts = ipath[i];//the cut start vertice
        cute = ipath[j];//the cut end vertice
        save = map_path.data[cuts][cute];//save the distance
        map_path.data[cuts][cute] = inf;//set the distance as -1
        searchshortest(start, end, &tempdis, temppath);//find the shortest path without the cut edge
        if ((*odis == 0 || *odis > tempdis) && temppath[0] != 0 && tempdis > idis)
        {
            for (k = 0; temppath[k] != 0; k++)
                opath[k] = temppath[k];//update the shortest path
            opath[k] = 0;
            *odis = tempdis;
        }
        map_path.data[cuts][cute] = save;//reload the distance
    }

    for (j = 0; ipath[j] != 0; j++)//search circle
    {
        DFS(j, j);
        for (k = 0; k < map_path.vertice; k++)
            visited[k] = 0;
    }
    if (circlept > 0)//there is at least one circle
    {
        int min = circledist[0];//select the min distance of the circles
        int minpt = 0;
        for (j = 1; j < circlept; j++)
        {
            if (circledist[j] < min)
                min = circledist[j], minpt = j;
        }
        min += idis;//add the shortest path distance
        if (min < *odis || *odis == 0)//compare with the normal 2nd shortest distance
        {
            *odis = min;//write the result
            for (j = 0, k = 0; ipath[j] != 0; j++, k++)//write the path
            {
                if (ipath[j] == circlepath[minpt][0])
                {
                    opath[k] = ipath[j];//copy the start
                    k++;
                    for (i = 1; circlepath[minpt][i] != ipath[j]; i++)//give out the circle
                    {
                        opath[k] = circlepath[minpt][i];
                        k++;
                    }
                    opath[k] = ipath[j];//copy the end
                }
                else opath[k] = ipath[j];//normally copy
            }
            opath[k] = 0;//end of path
        }
    }

    // for (j = 0; ipath[j] != 0; j++)//seek for the situation of back track
    // {
    //     targetv = ipath[j];//target vertice
    //     for (i = 1; i <= map_path.vertice; i++)//for all the vertices
    //     {
    //         if (map_path.data[targetv][i] >= 0)//there is a path
    //         {
    //             tempdis = idis + 2 * map_path.data[targetv][i];
    //             if (*odis == 0 || *odis > tempdis)//test the backtrack
    //             {
    //                 for (k = 0; k <= j; k++)//the same as the normal set
    //                     opath[k] = ipath[k];
    //                 opath[k] = i;
    //                 k++;
    //                 opath[k] = ipath[j];
    //                 k++;
    //                 for (; ipath[k - 2] != 0; k++)
    //                     opath[k] = ipath[k - 2];
    //                 opath[k] = 0;
    //                 *odis = tempdis;
    //             }
    //         }
    //     }
    // }
}

int main()
{
    initialize();//initialize the graph
    int inputstate = 0;
    cout << "please choose the way to build graph: manually: 1; random: 0" << endl;
    cin >> inputstate;
    if (inputstate == 1)//given the input method
        buildgraph();//build in manual
    else buildrandom();//build at random
    int tempdis;
    int i = 0;
    int* temppath = new int(map_path.vertice + 100);//the temp path
    int* finalpath = new int(map_path.vertice + 100);//the final result
    searchshortest(1, map_path.vertice, &tempdis, temppath);//search the shortest path
    if (temppath == NULL || tempdis == 0)//there is no path
    {
        cout << "there is no way to go from 1 to end." << endl;
        cout << "there is no 2nd shortest distance." << endl;
        exit(1);
    }
    cout << "shortest distance is: " << tempdis << endl;//output the distance
    cout << "the path is:";
    for (i = 0; temppath[i] != 0; i++)//output the path
        cout << " " << temppath[i];
    cout << endl;
    search2nd(1, map_path.vertice, temppath, tempdis, &tempdis, finalpath);//search the 2nd shortest path
    if (tempdis == 0)//there is no path
    {
        cout << "there is no 2nd shortest distance." << endl;
    }
    else
    {
        cout << "the 2nd shortest distance is: " << tempdis << endl;//output the distance
        cout << "the path is:";
        for (i = 0; finalpath[i] != 0; i++)//output the path
            cout << " " << finalpath[i];
    }
    
}