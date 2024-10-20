/*红黑树的原则如下：
1. 根节点为黑
2. 节点非黑即红
3. 红节点的子节点为黑
4. 叶子节点为黑（可不理会）
5. 叶子节点到根节点的最短路径黑高相同
*/

//此版本中，insert已经确认完全正确。

#include <bits/stdc++.h>
#define typename int
#define MAX_N 100000
using namespace std;

struct rbtree{
    typename data;
    rbtree* left;
    rbtree* right;
    rbtree* parent;
    char color; //先不用枚举类型，暂定为'r'时为红，为'b'时为黑。
};

rbtree* root = NULL;

bool find(typename insertion)
{
    rbtree* target = root;
    while (target != NULL)
    {
        if (insertion > target->data)
            target = target->right;
        else if (insertion < target->data)
            target = target->left;
        else return true;
    }
    return false;
}

void insert(typename insertion)
{
    if (root == NULL)
    {
        root = new rbtree;
        root->data = insertion;
        root->left = NULL;
        root->right = NULL;
        root->parent = NULL;
        root->color = 'b';
        return;
    }
    if (find(insertion))
    {
        cout << "the number " << insertion << " has been in the tree. It will be ignored." << endl;
        return;
    }
    rbtree* target = root;
    rbtree* newbranch = new rbtree;
    rbtree* subtree = NULL;
    rbtree* subroot = NULL;
    newbranch->data = insertion;
    newbranch->left = newbranch->right = NULL;
    newbranch->color = 'r';
    while (1)
    {
        if (target->data < insertion && target->right != NULL)
            target = target->right;
        else if (target->data > insertion && target->left != NULL)
            target = target->left;
        else if (target->data < insertion && target->right == NULL)
        {
            target->right = newbranch;
            newbranch->parent = target;
            break;
        }
        else if (target->data > insertion && target->left == NULL)
        {
            target->left = newbranch;
            newbranch->parent = target;
            break;
        }
    }

    target = newbranch;
    if (target->parent->color == 'b')//如果父节点为黑，直接返回即可
        return;
    
    //如果父节点为红，需要讨论：
    if (target->parent->parent == NULL)
    {
        cout << "ERROR!" << endl;
        exit(1);
    }

    //设：subtree为插入节点的父节点
    //subroot为插入节点的祖父节点
    //我们能够确定，subroot的颜色必须为黑，否则之前的插入是有问题的。
    subtree = target->parent;
    subroot = target->parent->parent;
    while (subroot != NULL)//直至遍历到根节点
    {
        if (subroot->left == subtree) //左侧
        {
            if (subroot->right != NULL && subroot->right->color == 'r')//这是b/r,r/r的情况
            {
                subroot->color = 'r';
                subroot->right->color = 'b';
                subtree->color = 'b';
            }
            else //这是b/r,b/r的情况
            {
                if (subtree->right == target) //r在内侧，因此要左旋
                {
                    subroot->left = target;
                    subtree->right = target->left;
                    subtree->parent = target;
                    target->left = subtree;
                    target->parent = subroot;

                    subtree = target; //与另一种情况统一
                    target = subtree->left;
                }
                //现在是b/r,b/r左的情况
                subtree->color = 'b';
                subroot->color = 'r';//改颜色后再做一次右旋，这里临时复用一下target作为subroot的父节点
                target = subroot->parent;
                if (target != NULL)
                {
                    if (target->left == subroot)
                        target->left = subtree;
                    else target->right = subtree;
                }
                else root = subtree;
                subroot->parent = subtree;
                subroot->left = subtree->right;
                subtree->parent = target;
                subtree->right = subroot;
                subroot = subtree;//这时subroot的颜色为黑色，那么必然不会影响到上层的树。
                break;
            }
        }
        else //右侧，复写左侧即可
        {
            if (subroot->left != NULL && subroot->left->color == 'r')//这是b/r,r/r的情况
            {
                subroot->color = 'r';
                subroot->left->color = 'b';
                subtree->color = 'b';
            }
            else //这是b/b,r/r的情况
            {
                if (subtree->left == target) //r在内侧，因此要右旋
                {
                    subroot->right = target;
                    subtree->left = target->right;
                    subtree->parent = target;
                    target->right = subtree;
                    target->parent = subroot;

                    subtree = target; //与另一种情况统一
                    target = subtree->right;
                }
                //现在是b/b,r/r右的情况
                subtree->color = 'b';
                subroot->color = 'r';//改颜色后再做一次左旋，这里临时复用一下target作为subroot的父节点
                target = subroot->parent;
                if (target != NULL)
                {
                    if (target->left == subroot)
                        target->left = subtree;
                    else target->right = subtree;
                }
                else root = subtree;
                subroot->parent = subtree;
                subroot->right = subtree->left;
                subtree->parent = target;
                subtree->left = subroot;
                subroot = subtree;//这时subroot的颜色为黑色，那么必然不会影响到上层的树。
                break;
            }
        }
        target = subroot;
        if (target->parent == NULL)//最简单的情况，只改颜色+直接到根节点了
        {
            target->color = 'b';
            break;
        }
        else if (target->parent->color == 'b')//此时上层视角为黑-红，符合红黑树
            break;
        else {
            subtree = target->parent;//更新subtree和subroot
            subroot = subtree->parent;//由于又出现了红-红，那么subroot必然是存在的。
        }
    }
}

void output()
{
    rbtree* onequeue[MAX_N];
    int start, end, lastend, counter;
    counter = 1;
    start = 0;
    end = 1;
    onequeue[0] = root;
    while (start != end)
    {
        cout << "level " << counter << ":";
        lastend = end;
        while (start != lastend)
        {
            if (onequeue[start] != NULL)
            {
                cout << "[" << onequeue[start]->data << onequeue[start]->color << "] ";
                onequeue[end] = onequeue[start]->left;
                end++;
                onequeue[end] = onequeue[start]->right;
                end++;
            }
            else {
                cout << "[nb] ";
            }
            start++;
        }
        cout << endl;
        counter++;
    }
}

void clear(typename insertion)
{
    cout << "the function doesn't finished." << endl;
    return;
    if (!find(insertion))
    {
        cout << "the number cannot be found in the tree." << endl;
        return;
    }
    rbtree* target = root;
    rbtree* subtree;
    rbtree* subroot;

    while (target != NULL)
    {
        if (insertion > target->data)
            target = target->right;
        else if (insertion < target->data)
            target = target->left;
        else break;
    }

    if (target == NULL)
    {
        cout << "ERROR." << endl;
        return;
    }

    subtree = target->parent;
    if (subtree != NULL)
        subroot = subtree->parent;
    else subroot = NULL;
    if (target->color == 'r')//删除的节点为红色
    {
        if (target->left == NULL && target->right == NULL)//删除的节点为叶子节点
        {
            if (subtree == NULL)
            {
                delete target;
                root = NULL;
            }
            if (subtree->left == target)
                subtree->left = NULL;
            else if (subtree->right == target)
                subtree->right = NULL;
            delete target;
        }
        else if ((target->left != NULL && target->right == NULL) || (target->left == NULL && target->right != NULL))
        {//删除的节点只有一个子节点
            cout << "ERROR." << endl;//这种情况不会发生。
            return;
        }
        else{
        }
    }
    else{//删除的节点为黑色
    }
    
}

int main()
{
    int n;
    string opt;
    typename x;
    while (1)
    {
        cout << "please input the operation: insert/delete/exit" << endl;
        cin >> opt;
        if (opt == "insert")
        {
            cout << "please input the whole number of the insertion:" << endl;
            cin >> n;
            cout << "please input the data:" << endl;
            for (int i = 0; i < n; i++)
            {
                cin >> x;
                insert(x);
            }
            output();
        }
        else if (opt == "delete")
        {
            cout << "the present tree is like below:" << endl;
            output();
            cout << "please input the number to be deleted:" << endl;
            cin >> x;
            clear(x);
        }
        else if (opt == "exit")
            break;
        else cout << "this is a wrong operation, input the operation again." << endl;
    }
}

//10
//3 1 4 5 9 2 6 8 7 0
