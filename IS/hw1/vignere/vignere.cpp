#include <bits/stdc++.h>
using namespace std;
//cipher text: 
// ktb uel ueg vit nth uex mon veg gmr cgx ptl yhh jao gch oem qch pdn etx upb qnt iet iab psm
// aon cnw vou tiu gta gmm qsx tvx aon iio gta gmb psm tuv vih pst pdv crx hok vhx ota wsw quu
// new cgx ptl crx tev tub vew cnw wsx fsn pts wta gak voy yak
// ktbueluegvitnthuexmonveggmrcgxptlyhhjaogchoemqchpdnetxupbqntietiabpsm
// aoncnwvoutiugtagmmqsxtvxaoniiogtagmbpsmtuvvihpstpdvcrxhokvhxotawswquu
// newcgxptlcrxtevtubvewcnwwsxfsnptswtagakvoyyak

// ktbueluegvitnthuexmonveggmrcgxptlyhhjaogchoemqchpdnetxupbqntietiabpsmaoncnwvoutiugtagmmqsxtvxaoniiogtagmbpsmtuvvihpstpdvcrxhokvhxotawswquunewcgxptlcrxtevtubvewcnwwsxfsnptswtagakvoyyak

// for 'ue', 'ao', 'ak' repeats every 3 characters

void tripletest(string cipher)
{
    string pair1, pair2;
    for (int i = 0; i < cipher.length() - 3; i++)
    {
        pair1 = cipher.substr(i, 3);
        for (int j = i + 1; j <= cipher.length() - 3; j++)
        {
            pair2 = cipher.substr(j, 3);
            if (pair1 == pair2)
            {
                cout << pair1 << ": " << j - i;
                for (int k = 2; k < j - i; k++)
                {
                    if ((j - i) % k == 0)
                        cout << " " << k; 
                }
                cout << endl;
            }
        }
    }
}
/*the answer is:
cgx: 114 2 3 6 19 38 57
gxp: 114 2 3 6 19 38 57
xpt: 114 2 3 6 19 38 57
ptl: 114 2 3 6 19 38 57
bps: 39 3 13
psm: 39 3 13
aon: 24 2 3 4 6 8 12
cnw: 87 3 29
gta: 18 2 3 6 9
tag: 18 2 3 6 9
tag: 90 2 3 5 6 9 10 15 18 30 45
agm: 18 2 3 6 9
tag: 72 2 3 4 6 8 9 12 18 24 36
crx: 27 3 9
ewc: 18 2 3 6 9

3在所有的重复组合中均出现过，因此暂定重复密钥长度为3.
*/

void frequency(int x, string cipher){
    int alphabet[26];
    string* subcipher = new string[x];
    for (int i = 0; i < x; i++){
        for (int j = i; j < cipher.size(); j += 3){
            subcipher[i] += cipher[j];
        }
    }
    for (int i = 0; i < x; i++){
        for (int j = 0; j < 26; j++){
            alphabet[j] = 0;
        }
        for (int j = 0; j < subcipher[i].size(); j++){
            alphabet[subcipher[i][j] - 'a']++;
        }
        for (int j = 0; j < 26; j++){
            cout << "times of " << (char)(j + 'a') << ": " << alphabet[j] << endl;
        }
    }
}
/*经frequency后，提取出频率最大的6个字母：
list1: 
p:8
g:7
v:7
c:6
t:5
list2:
t:10
e:9
s:7
o:6
a:4
i:4
list3:
x:10
w:5
b:5
h:5
n:5
w:5
*/

void caesar(int x, string cipher, string key){
    string* subcipher = new string[x];
    for (int i = 0; i < x; i++){
        for (int j = i; j < cipher.size(); j += 3){
            subcipher[i] += cipher[j];
        }
    }
    for (int i = 0; i < x; i++){
        for (int j = 0; j < subcipher[i].size(); j++){
            subcipher[i][j] += 'e' - key[i];
            if (subcipher[i][j] < 'a')
                subcipher[i][j] += 26;
            if (subcipher[i][j] > 'z')
                subcipher[i][j] -= 26;
        }
    }
    for (int i = 0; i < subcipher[0].size(); i++){
        for (int j = 0; j < x; j++){
            cout << subcipher[j][i];
        }
    }
}
//经过几次尝试，发现为gex->e为密钥（之后可以转换一下）：
/*
it is essential to seek out enemy agents who have come to conduct espionage(间谍活动) against you 
and to bribe(贿赂) them to serve you give them instructions 
and care for them thus doubled agents are recruited and used sun tzu the art of war(孙子兵法（6，这玩意居然这么断句）)
*/


int main()
{
    string cipher;
    //cin >> cipher;
    cipher = "ktbueluegvitnthuexmonveggmrcgxptlyhhjaogchoemqchpdnetxupbqntietiabpsmaoncnwvoutiugtagmmqsxtvxaoniiogtagmbpsmtuvvihpstpdvcrxhokvhxotawswquunewcgxptlcrxtevtubvewcnwwsxfsnptswtagakvoyyak";
    //cipher = "abcabcabc";
    //tripletest(cipher);
    //frequency(3, cipher);
    caesar(3, cipher, "gex");
}