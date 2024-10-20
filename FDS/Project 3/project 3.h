#ifndef PROJECT_3
#define PROJECT_3

#include <bits/stdc++.h>
using namespace std;

//use matrix to store the graph
//the value of mapgraph[a][b] means the distance between a and b, if distance is 0, a is not connected with b
struct mapgraph
{
    int vertice;//the vertice of the graph
    int edge;//the edge of the graph
    int data[1100][1100];//the data of the graph
};

mapgraph map_path;//the main data structure
int shortestpath[1100];//the shortest path


void initialize();//initialize the graph
void buildgraph();//build the graph
void buildrandom();//build at random
void searchshortest(int start, int end, int* odis, int* opath);//search the shortest path
void search2nd(int start, int end, int* ipath, int idis, int* odis, int* opath);//search 2nd shortest path




#endif