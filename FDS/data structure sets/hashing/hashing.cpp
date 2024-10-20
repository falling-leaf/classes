//数学建模被爆掉了，很烦，不打double hashing了
//double hashing 第二个函数是取小于size的最大素数R，H = R - (x % R)
//之后跟quadratic 一样
//注意：起始的第一次哈希函数不一定是target % size，本程序全部设为了这个函数
//QAQ 4.5学分啊，4.5学分啊！！！
#include <bits/stdc++.h>
#define MAX_N 100;
using namespace std;

struct Hash
{
    int size;
    int* table;
};
Hash onehash;

void hashing()
{
    int state;
    cout << "please input the function of hashing: " << endl;
    cout << "linear: 1; quadratic: 2; double: 3" << endl;
    cin >> state;
    while (state != 1 && state != 2 && state != 3)
    {
        cout << "please input the function code: " << endl;
        cin >> state;
    }
    int num, target, pos, save;
    bool notfull;
    cout << "please input the size of table: " << endl;
    cin >> onehash.size;
    cout << "please input the number of data: " << endl;
    cin >> num;
    cout << "please input the data: " << endl;
    onehash.table = new int[onehash.size];
    for (int i = 0; i < onehash.size; i++)
        onehash.table[i] = -1;
    for (int i = 0; i < num; i++)
    {
        notfull = false;
        cin >> target;
        pos = target % onehash.size;
        save = pos;
        if (state == 1)
        {
            for (int j = 0; j < onehash.size; j++)
            {
                if (onehash.table[pos] == -1)
                {
                    notfull = true;
                    break;
                }
                pos++;
                if (pos == onehash.size - 1)
                    pos = 0;
            }
        }
        else if (state == 2)
        {
            for (int j = 1; j < onehash.size / 2; j++)
            {
                if (onehash.table[pos] == -1)
                {
                    notfull = true;
                    break;
                }
                pos = pos % onehash.size + j * j;
                pos = pos % onehash.size;
            }
        }
        else if (state == 3)
        {

        }
        if (notfull)
            onehash.table[pos] = target;
        else cout << "the table is full; data " << target << " cannot be inserted." << endl;
    }
    cout << "the value of hash table is: " << endl;
    for (int i = 0; i < onehash.size; i++)
        cout << onehash.table[i] << " ";
}

int main()
{
    hashing();
}