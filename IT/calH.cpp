#include <bits/stdc++.h>
using namespace std;
int main()
{
    cout << "input the number of n" << endl;
    int n;
    cin >> n;
    double x;
    double sum = 0.0;
    for (int i = 0; i < n; i++){
        cin >> x;
        sum -= x * log2(x);
    }
    cout << sum << endl;
}