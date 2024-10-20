//关于树
//以给出前序、中序排列为例构建树
#include <bits/stdc++.h>
#define MAX_N 20
using namespace std;

struct Tree
{
    int data;
    Tree* left;
    Tree* right;
};
Tree* onetree;
int size;

//关键函数：建树
Tree* buildtree(int* in, int* pre, int n)
{
    if (n == 0)
        return NULL;
    Tree* t = (Tree*)malloc(sizeof(Tree));
    t->data = pre[0];
    int i = 0;
    for (i = 0; in[i] != t->data; i++);
    t->left = buildtree(in, pre + 1, i);
    t->right = buildtree(in + i + 1, pre + i + 1, n - i - 1);
}

//关键函数：树的前序遍历
void preoutput(Tree* t)
{
    if (t == NULL)
        return;
    cout << t->data << " ";
    preoutput(t->left);
    preoutput(t->right);
}

//关键函数：树的中序遍历
void inoutput(Tree* t)
{
    if (t == NULL)
        return;
    inoutput(t->left);
    cout << t->data << " ";
    inoutput(t->right);
}

//关键函数：树的后序遍历
void postoutput(Tree* t)
{
    if (t == NULL)
        return;
    postoutput(t->left);
    postoutput(t->right);
    cout << t->data << " ";
}

//关键函数：树的层级遍历
void leveloutput(Tree* t)
{
    
}

int main()
{
    int preorder[MAX_N];
    int inorder[MAX_N];
    cin >> size;
    for (int i = 0; i < size; i++)
        cin >> preorder[i];
    for (int i = 0; i < size; i++)
        cin >> inorder[i];
    onetree = buildtree(inorder, preorder, size);
    preoutput(onetree);
    cout << endl;
    inoutput(onetree);
    cout << endl;
    postoutput(onetree);
    cout << endl;
}