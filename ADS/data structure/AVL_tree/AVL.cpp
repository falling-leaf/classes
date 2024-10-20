//这是PTA给出的题目实例，要求输出AVL树处理过后的根节点。
//最大的感触：不要乱用NULL！真实的情况要比实例复杂很多，尽可能假设填满所有节点
//parent关系定义不明确，重新把关系梳理一下比较好
#include <bits/stdc++.h>
using namespace std;
struct onetree
{
    onetree* parent;//父节点（额外需求）
    onetree* left;//左节点
    onetree* right;//右节点
    int data;//内容
};

onetree* root = NULL;//根节点

//关键函数：求一个树的高度（思路与层次遍历相似）
int findheight(onetree* pt)
{
    if (pt == NULL)//若节点为空，高度为0
        return 0;
    onetree* onequeue[1000];//建立临时队列
    onetree* temp;
    int startpt = 0;
    int endpt = 1;
    int level = 0;
    int lastend = 1;
    onequeue[0] = pt;
    while (startpt != endpt)//直至队列中没有节点为止
    {
        lastend = endpt;//记录上一个层级中的节点
        level++;//层级可加
        while (startpt != lastend)//输出所有的上层级节点
        {
            temp = onequeue[startpt];
            startpt++;
            if (temp != NULL)//将节点的左右节点入队列
            {
                if (temp->left != NULL)
                {
                    onequeue[endpt] = temp->left;
                    endpt++;
                }
                if (temp->right != NULL)
                {
                    onequeue[endpt] = temp->right;
                    endpt++;
                }
            }
        }
    }
    return level;
}

//关键函数：AVLtree，包括BST插入及AVL调整
void AVLtree(int target)
{
    if (root == NULL)//根节点为空，直接写入根节点
    {
        root = new onetree;
        root->left = NULL;
        root->right = NULL;
        root->parent = NULL;
        root->data = target;
        return;
    }
    onetree* temp = new onetree;//创造新的节点
    temp->left = NULL;
    temp->right = NULL;
    temp->parent = NULL;
    temp->data = target;
    onetree* compare = root;
    while (compare != NULL)//开始BST插入
    {
        if (target < compare->data)//如果比当前节点小
        {
            if (compare->left == NULL)//无左节点则做左节点
            {
                temp->parent = compare;
                compare->left = temp;
                break;
            }
            else compare = compare->left;//否则进left
        }
        else
        {
            if (compare->right == NULL)//无右节点则做右节点
            {
                temp->parent = compare;
                compare->right = temp;
                break;
            }
            else compare = compare->right;//否则进right
        }
    }

    //开始AVL调整
    compare = temp;//compare指向现在的节点，从下往上调整
    onetree* subroot;
    int bfstate[10000];//记录bf的状态
    int bfpt = 0;
    int tempa, tempb;
    while (compare != NULL)//root的parent是NULL
    {
        tempa = findheight(compare->left);//求左子树的高度
        tempb = findheight(compare->right);//求右子树的高度
        bfstate[bfpt] = tempa - tempb;//求它的bf值
        if (bfstate[bfpt] == 2)//第一层为L
        {
            if (bfstate[bfpt - 1] == 1)//是LL
            {
                if (root == compare)//若为根节点，则根节点更改
                    root = compare->left;
                else //否则将此部分根节点续在父节点上
                {
                    if (compare->parent->left == compare)
                        compare->parent->left = compare->left;
                    else compare->parent->right = compare->left;
                }
                subroot = compare->left;
                subroot->parent = compare->parent;
                compare->parent = subroot;
                compare->left = subroot->right;
                subroot->right = compare;
            }
            else if (bfstate[bfpt - 1] == -1)
            {
                if (root == compare)
                    root = compare->left->right;
                else 
                {
                    if (compare->parent->left == compare)
                        compare->parent->left = compare->left->right;
                    else compare->parent->right = compare->left->right;
                }
                subroot = compare->left->right;
                subroot->parent = compare->parent;
                compare->left->right = subroot->left;
                compare->left->parent = subroot;
                subroot->left = compare->left;
                compare->left = subroot->right;
                compare->parent = subroot;
                subroot->right = compare;
            }
        }
        else if (bfstate[bfpt] == -2)
        {
            if (bfstate[bfpt - 1] == -1)
            {
                if (root == compare)
                    root = compare->right;
                else 
                {
                    if (compare->parent->right == compare)
                        compare->parent->right = compare->right;
                    else compare->parent->left = compare->right;
                }
                subroot = compare->right;
                subroot->parent = compare->parent;
                compare->parent = subroot;
                compare->right = subroot->left;
                subroot->left = compare;
            }
            else if (bfstate[bfpt - 1] == 1)
            {
                if (root == compare)
                    root = compare->right->left;
                else
                {
                    if (compare->parent->right == compare)
                        compare->parent->right = compare->right->left;
                    else compare->parent->left = compare->right->left;
                }
                subroot = compare->right->left;
                subroot->parent = compare->parent;
                compare->right->left = subroot->right;
                compare->right->parent = subroot;
                subroot->right = compare->right;
                compare->right = subroot->left;
                compare->parent = subroot;
                subroot->left = compare;
            }
        }
        compare = compare->parent;
        bfpt++;
    }
}

int main()
{
    int n, q;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> q;
        AVLtree(q);
    }
    cout << root->data << endl;
}