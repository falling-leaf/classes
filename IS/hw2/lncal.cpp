//this is calculations for large number:
//we suppose that the large number is smaller than 10^(2^32), 
//which means the range of figure is out of "int".
//in the division algorithm, we only concern the quotient
/*由于实验2不要求写报告，在这里注明整体的思路与流程：
打代码的时候不知道哪根筋抽了，并没有用int[]来进行描述而是用string描述
这导致所有运算都要考虑ASCII和数字之间的转换，导致这个代码长得很抽象

具体的使用方法为：
先输入想要验证的作业：1/2

再输入对应的数据：
例如：
1
666666666666666666666666666666666666666
33333333333333333
输出为sample1对应的结果

2
353
3
97
233
输出为sample2对应的结果

*/
#include <bits/stdc++.h>
using namespace std;

bool lcompare(string n1, string n2){//写一个正数的比较函数，如果n1>=n2则返回true
    while (n1[0] == '0' && n1.length() != 1){
        n1 = n1.substr(1, n1.length() - 1);
    }
    while (n2[0] == '0' && n2.length() != 1){
        n2 = n2.substr(1, n2.length() - 1);
    }
    if (n1.length() > n2.length())
        return true;
    else if (n1.length() < n2.length())
        return false;
    else {
        for (int i = 0; i < n1.length(); i++){
            if (n1[i] < n2[i])
                return false;
            else if (n1[i] > n2[i])
                return true;
        }
        return true;
    }
}

string ladd(string n1, string n2){
    string temp;
    bool minus1, minus2, resultsign;
    minus1 = minus2 = false;
    if (n1[0] == '-'){
        minus1 = true;
        n1 = n1.substr(1, n1.length() - 1);
    }

    if (n2[0] == '-'){
        minus2 = true;
        n2 = n2.substr(1, n2.length() - 1);
        }

    if (n1.length() > n2.length()){
        temp = n2;
        n2 = n1;
        n1 = temp;
        resultsign = minus1;
        minus1 = minus2;
        minus2 = resultsign;
    }
    string zero = "0";
    for (int i = 0; i < (n2.length() - n1.length()); i++){
        zero += "0";
    }
    n1 = zero + n1;
    n2 = "0" + n2;

    if (minus1 ^ minus2){
        if (n1 == n2)
            return "0";
        if ((minus1 && lcompare(n1, n2)) || (minus2 && !lcompare(n1, n2)))
            resultsign = true;
        else resultsign = false;
        if (!lcompare(n1, n2)){
            temp = n2;
            n2 = n1;
            n1 = temp;
        }
        for (int i = n1.length() - 1; i >= 0; i--){
            n1[i] = n1[i] - n2[i] + '0';
            if (n1[i] < '0'){
                n1[i - 1]--;
                n1[i] += 10;
            }
        }
        while (n1[0] == '0')
            n1 = n1.substr(1, n1.length() - 1);
        if (resultsign && n1 != "0")
            n1 = "-" + n1;
    }
    else {
        for (int i = n1.length() - 1; i >= 0; i--){
            n1[i] += n2[i] - '0';
            if (n1[i] > '9'){
                n1[i] -= 10;
                n1[i - 1]++;//because the first line is 0/0,so it is safe
            }
        }
        while (n1[0] == '0')
            n1 = n1.substr(1, n1.length() - 1);
        if (minus1 && n1 != "0")
            n1 = "-" + n1;
    }
    return n1;
}

string lsub(string n1, string n2){
    if (n1 == n2)
        return "0";
    if (n2[0] == '-'){
        n2 = n2.substr(1, n2.length() - 1);
    }
    else n2 = "-" + n2;
    return ladd(n1, n2);
}

string lmult(string n1, string n2){
    bool minus = false;
    int times = 0;//because times must be between 0 and 9, so "int" can be used.
    if (n1[0] == '-'){
        minus = !minus;
        n1 = n1.substr(1, n1.length() - 1);
    }
    if (n2[0] == '-'){
        minus = !minus;
        n2 = n2.substr(1, n2.length() - 1);
    }

    string n3 = "0";
    for (int i = n2.length() - 1; i >= 0; i--){
        times = n2[i] - '0';
        for (int j = 0; j < times; j++)
            n3 = ladd(n1, n3);
        n1 += "0";
    }

    if (minus && n3 != "0")
        n3 = "-" + n3;
    return n3;
}

string ldiv(string n1, string n2){
    bool minus = false;
    string counter = "0"; //the counter is sure to be between 0 and 9
    string n3 = "0";
    string subn1;
    if (n1[0] == '-'){
        minus = !minus;
        n1 = n1.substr(1, n1.length() - 1);
    }
    if (n2[0] == '-'){
        minus = !minus;
        n2 = n2.substr(1, n1.length() - 1);
    }
    if (n2 == "0")//the divider is 0
        return "ERROR";
    if (!lcompare(n1, n2)){
        return "0";
    }

    for (int i = 0; i < n1.length(); i++){
        subn1 += n1[i];
        if (!lcompare(subn1, n2))
        {
            n3 += "0";
            continue;
        }
        for (counter = "0"; lcompare(subn1, n2); counter[0]++){
            subn1 = lsub(subn1, n2);
        }
        n3 += counter;
    }

    while (n3[0] == '0')
        n3 = n3.substr(1, n3.length() - 1);
    if (minus)
        n3 = "-" + n3;
    return n3;
}

string lmod(string n1, string n2){
    string n3 = ldiv(n1, n2);
    return lsub(n1, lmult(n2, n3));
}

string lsquare(string n1, string n2){
    if (n2[0] == '-')
        return "ERROR";
    if (n2 == "0")
        return "1";
    string half, remain;
    string tempresult = "1";
    while (lcompare(n2, "3"))
    {
        half = ldiv(n2, "2");
        remain = lmod(n2, "2");
        if (remain == "1"){
            tempresult = lmult(tempresult, n1);
        }
        n2 = half;
        n1 = lmult(n1, n1);
    }
    for (string i = "1"; lcompare(n2, i); n2 = lsub(n2, "1"))
        tempresult = lmult(tempresult, n1);
    return tempresult;
}

int main()
{
    int n = 0;
    do {
    cout << "please input the hw you want to check: 1: hw2.1; 2: hw2.2" << endl;
    cin >> n;
    }while (n != 1 && n != 2);
    if (n == 1){
        string n1, n2;
        cout << "please input the two numbers you want to calculate:" << endl;
        cout << "note: the number can't include float point." << endl;
        cin >> n1 >> n2;
        cout << ladd(n1, n2) << endl;
        cout << lsub(n1, n2) << endl;
        cout << lmult(n1, n2) << endl;
        cout << ldiv(n1, n2) << endl;
        cout << lmod(n1, n2) << endl;
    }
    else if (n == 2){
        string p, g, apriv, bpriv, apub, bpub, session;
        cout << "please input p/g/A's private key/B's private key" << endl;
        cin >> p >> g >> apriv >> bpriv;
        apub = lmod(lsquare(g, apriv), p);
        bpub = lmod(lsquare(g, bpriv), p);
        cout << "A's public key: " << apub << endl;
        cout << "B's public key: " << bpub << endl;
        session = lmod(lsquare(apub, bpriv), p);
        cout << "Session key: " << session << endl;
    }
}

/*sample input:
666666666666666666666666666666666666666
33333333333333333
the output is the same as the sample output.
*/

/*在测试时发现需要优化乘方运算的算法，跑的太慢了
优化乘方后，发现除法也好慢，再改除法
最终发现是比较大小的时候通过数位比较时，字符串前面有0
*/