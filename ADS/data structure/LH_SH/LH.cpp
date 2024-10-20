//这是写leftist heap/stew heap的实现的程序
//以小根堆为例
#include <bits/stdc++.h>
#define MAX_N 1000
using namespace std;

struct lh{
    int data;
    lh* left;
    lh* right;
    int Npl;
};

lh* merge(lh* h1, lh* h2){
    if (h2 == NULL)
        return h1;
    if (h1 == NULL)
        return h2;
    lh* temp;
    int tempNpl;
    if (h2->data < h1->data){//小根堆
        temp = h2;
        h2 = h1;
        h1 = temp;
    }//现在统一为要把h2插到h1中
    if (h1->left == NULL)
        h1->left = h2;
    else {
        temp = h1->right;
        h1->right = h2;
        h2 = temp;
        merge(h1->right, h2);
        if (h1->left->Npl < h1->right->Npl){
            tempNpl = h1->left->Npl;
            h1->left->Npl = h1->right->Npl;
            h1->right->Npl = tempNpl;
        }
        h1->Npl = h1->right->Npl + 1;
    }
    return h1;
}

void insert(lh* h1, int data){
    lh* h2 = new lh;
    h2->data = data;
    h2->left = h2->right = NULL;
    h2->Npl = 0;
    h1 = merge(h1, h2);
}

void output(lh* h){
    lh* onequeue[MAX_N];
    int start, end;
}


int main()
{
    
}