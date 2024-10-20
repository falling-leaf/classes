#include <bits/stdc++.h>
using namespace std;
//cipher text:FBUQIUUDSHOFJOEKHDQCUMYJXJXUIQCUAUOQDTKFBEQTJEBUQHDYDWYDPZK
int main()
{
    string ciphertext;
    cin >> ciphertext;
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; ciphertext[j] != '\0'; j++)
        {
            ciphertext[j]++;
            if (ciphertext[j] > 'z' || (ciphertext[j] > 'Z' && ciphertext[j] < 'a'))
                ciphertext[j] -= 26;
        }
        cout << ciphertext << endl;
    }
}
//plain text: PLEASE ENCRYPT YOUR NAME WITH THE SAME KEY AND UPLOAD TO LEARNING IN ZJU (key: A = K)
//plain text: wanghaoyuan
//cipher text: mqdwxqeokqd