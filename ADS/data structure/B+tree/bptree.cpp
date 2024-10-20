//这个程序中，我们暂且设-1不会出现，作为空白。
//当parent != NULL时，要排序！
//希望能够给出广义上的order的执行方式，因此用宏来定义order
#include <bits/stdc++.h>
#define order 3
using namespace std;

struct bptree{
    int data[order + 1];
    bptree* children[order + 1];
    bptree* parent;
};
bptree* root = NULL;

bool judgeleaf(bptree* pt)//判断叶子节点（无子节点）
{
    for (int i = 0; i < order; i++)
    {
        if (pt->children[i] != NULL)
            return false;
    }
    return true;
}

bool find(int x)//找x
{
    bptree* target = root;
    if (target == NULL)
        return false;
    while (!judgeleaf(target))//寻找x应该在的叶子节点
    {
        for (int i = 0; i < order; i++)
        {
            if (x < target->data[i] || target->data[i] == -1)
            {
                target = target->children[i];
                break;
            }
        }
    }
    for (int i = 0; i < order; i++)
    {
        if (target->data[i] == x)
            return true;
    }
    return false;
}

void adjust(bptree* target)//这里无法做到order任意
{
    int mid;
    int newkey;
    bool endsign = false;

    while (1)
    {
        if (judgeleaf(target))
            mid = (order + 1) / 2;
        else mid = order / 2;
        newkey = target->data[mid];
        bptree* left = new bptree;//作为新根节点的左节点
        for (int i = 0; i <= order; i++)//初始化
        {
            left->data[i] = -1;
            left->children[i] = NULL;
        }
        for (int i = 0; i < mid; i++)//设置left
        {
            left->data[i] = target->data[i];
            left->children[i] = target->children[i];
            if (target->children[i] != NULL)
                target->children[i]->parent = left;
        }
        left->children[mid] = target->children[mid];
        if (target->children[mid] != NULL)
            target->children[mid]->parent = left;
        left->parent = target->parent;
        for (int i = mid, k = 0; k <= order; i++, k++)
        {
            if (!judgeleaf(target) && i == mid)
                i++;
            if (i <= order)
                target->data[k] = target->data[i];
            else target->data[k] = -1;
        }
        for (int i = mid + 1, k = 0; k <= order; i++, k++)
        {
            if (i <= order)
                target->children[k] = target->children[i];
            else target->children[k] = NULL;
        }
        if (target->parent == NULL)//root作为新根节点的右节点
        {
            bptree* another = new bptree;//作为新根节点
            for (int i = 0; i <= order; i++)
            {
                another->data[i] = -1;
                another->children[i] = NULL;
            }
            another->data[0] = newkey;//这是mid提取出的根节点
            another->children[0] = left;//left中元素比mid小
            another->children[1] = target;//root中元素比mid大
            target->parent = another;
            left->parent = another;
            root = another;
            endsign = true;
        }
        else {
            int i = 0;
            int temp1, temp2;
            temp2 = newkey;
            bptree* temppt;
            bptree* subroot = target->parent;
            for (i = 0; subroot->data[i] < newkey && subroot->data[i] != -1; i++);
            for (; i <= order; i++)
            {
                temp1 = target->parent->data[i];
                target->parent->data[i] = temp2;
                temp2 = temp1;
            }
            for (i = 0; subroot->children[i] != NULL; i++)
            {
                if (subroot->children[i]->data[0] >= newkey)
                    break;
            }
            for (; i <= order; i++)
            {
                temppt = target->parent->children[i];
                target->parent->children[i] = left;
                left = temppt;
            }
            target = target->parent;
            if (target->data[order - 1] == -1)
                endsign = true;
        }
        if (endsign)
            break;
    }
}

void insert(int x)//插x
{
    if (root == NULL)//如果还没初始化root，初始化之
    {
        root = new bptree;
        for (int i = 0; i <= order; i++)
        {
            root->data[i] = -1;
            root->children[i] = NULL;
        }
        root->parent = NULL;
        root->data[0] = x;
        return;
    }

    bptree* target = root;
    bool exception = true;
    int temp;
    while (!judgeleaf(target))//寻找x应该在的叶子节点
    {
        for (int i = 0; i < order; i++)
        {
            if (x < target->data[i] || target->data[i] == -1)
            {
                target = target->children[i];
                break;
            }
        }
    }
    for (int i = 0; i <= order; i++)
    {
        if (target->data[i] == -1 && i != order)
        {
            exception = false;
            target->data[i] = x;
            break;
        }
        else {
            if (target->data[i] > x)
            {
                temp = x;
                x = target->data[i];
                target->data[i] = temp;
            }
        }
    }
    if (exception)
    {
        target->data[order] = x;
        adjust(target);
    }
}

void output()
{
    bptree* onequeue[100000];
    bptree* target;
    int start, end, rend;
    onequeue[0] = root;
    start = 0;
    end = 1;
    while (start != end)
    {
        rend = end;
        while (start != rend)
        {
            target = onequeue[start];
            cout << "[" << target->data[0];
            for (int i = 1; i < order; i++)
            {
                if (target->data[i] == -1)
                    break;
                cout <<  "," << target->data[i];
            }
            cout << "]";
            for (int i = 0; i < order; i++)
            {
                if (target->children[i] == NULL)
                    break;
                onequeue[end] = target->children[i];
                end++;
            }
            start++;
        }
        cout << endl;
    }
}

int main()
{
    int n, x;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> x;
        if (find(x))
            cout << "Key " << x << " is duplicated" << endl;
        else insert(x);
    }
    output();
}