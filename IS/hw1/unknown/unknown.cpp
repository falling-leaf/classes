#include <bits/stdc++.h>
using namespace std;
/*
cipher text: MAL TIRRUEZF CR MAL RKZYIOL EX MAL OIY UAE RICF "MAL ACWLRM DYEUPLFWL CR ME DYEU MAIM UL IZL RKZZEKYFLF GH OHRMLZH"
*/

void caesar(string cipher){
    for (int i = 0; i < 26; i++){
        for (int j = 0; j < cipher.size(); j++){
            if (cipher[j] >= 'A' && cipher[j] <= 'Z'){
                cipher[j]++;
                if (cipher[j] > 'Z'){
                    cipher[j] -= 26;
                }
            }
        }
        cout << cipher << endl;
    }
}
//凯撒并没有什么卵用

//重新想，MAL重复很多，根据分隔符也可以明白它是一个单词，这说明加密方式是替换而非位移
//那么频率分析等等都可以用，确定一对替换就可以换所有的字母

void frequency(string cipher){
    int alphabet[26];
    for (int i = 0; i < 26; i++){
        alphabet[i] = 0;
    }
    for (int i = 0; i < cipher.size(); i++){
        alphabet[cipher[i] - 'A']++;
    }
    for (int i = 0; i < 26; i++){
        cout << "letter " << (char)(i + 'A') << ": " << alphabet[i] << endl;
    }
}
/* 结果如下：
letter A: 7
letter B: 0
letter C: 4
letter D: 2
letter E: 7
letter F: 5
letter G: 1
letter H: 3
letter I: 6
letter J: 0
letter K: 3
letter L: 12
letter M: 9
letter N: 0
letter O: 3
letter P: 1
letter Q: 0
letter R: 9
letter S: 0
letter T: 1
letter U: 5
letter V: 0
letter W: 2
letter X: 1
letter Y: 5
letter Z: 6
*/

/*
则L大概率是e，以e结尾频繁出现的单词，一般为冠词/介词，可以猜一手the
那么MAL->THE:
MAL TIRRUEZF CR MAL RKZYIOL EX MAL OIY UAE RICF "MAL ACWLRM DYEUPLFWL CR ME DYEU MAIM UL IZL RKZZEKYFLF GH OHRMLZH"
变为：
THE ________ __ THE ______E __ THE ___ _H_ ____ "THE H__E_T _____E__E __ T_ ____ TH_T _E __E ________E_ __ ___TE__"
显然MAIM应该是that（用元音字母换一下即可），那么I->A:
THE _A______ __ THE ____A_E __ THE _A_ _H_ _A__ "THE H__E_T _____E__E __ T_ ____ THAT _E A_E ________E_ __ ___TE__"
再看ME，显然那个词应该是to（原理同上），那么E->O:
THE _A___O__ __ THE ____A_E O_ THE _A_ _HO _A__ "THE H__E_T __O__E__E __ TO __O_ THAT _E A_E ____O___E_ __ ___TE__"
26个字母顺一遍，_HO必定是who，那么可以确定U->W:
THE _A__WO__ __ THE ____A_E O_ THE _A_ WHO _A__ "THE H__E_T __OW_E__E __ TO __OW THAT WE A_E ____O___E_ __ ___TE__"
右侧句子顺下来显然应该是that we are ..., 则Z->R:
THE _A__WOR_ __ THE __R_A_E O_ THE _A_ WHO _A__ "THE H__E_T __OW_E__E __ TO __OW THAT WE ARE __RRO___E_ __ ___TER_"
感觉做不下去了，回头看一眼频率分布图：
E-T-A-O-I-N-H-S-R-D-L
现在还有I/N/S/D/L没找到
看R的位置，首先它有CR这个单词（排除D/L），而RICF对应的明文中显然说明R为辅音字母（排除I）
这个时候，不论R是N还是S，C都必然确定是I了（A/E/O确定，U不可能）
那么C->I:
THE _A__WOR_ I_ THE __R_A_E O_ THE _A_ WHO _AI_ "THE HI_E_T __OW_E__E I_ TO __OW THAT WE ARE __RRO___E_ __ ___TER_"
_AI_是动词，后面跟双引号，不妨猜测其为said，R恰好对应之前猜测的范围内
那么R->S, F->D:
THE _ASSWORD IS THE S_R_A_E O_ THE _A_ WHO SAID "THE HI_EST __OW_ED_E IS TO __OW THAT WE ARE S_RRO__DED __ __STER_"
显然第二个词是password,改掉它：
THE PASSWORD IS THE S_R_A_E O_ THE _A_ WHO SAID "THE HI_EST __OW_ED_E IS TO __OW THAT WE ARE S_RRO__DED __ __STER_"
此时有两个想法：猜测__OW是什么，或者S_RRO__DED是什么，猜出第二个应该是surrounded:
那么K->U, Y->N:
MAL TIRRUEZF CR MAL RKZYIOL EX MAL OIY UAE RICF "MAL ACWLRM DYEUPLFWL CR ME DYEU MAIM UL IZL RKZZEKYFLF GH OHRMLZH"
THE PASSWORD IS THE SURNA_E O_ THE _AN WHO SAID "THE HI_EST _NOW_ED_E IS TO _NOW THAT WE ARE SURROUNDED __ __STER_"
加上这个线索，可以猜出_NOW是know，则D->K:
THE PASSWORD IS THE SURNA_E O_ THE _AN WHO SAID "THE HI_EST KNOW_ED_E IS TO KNOW THAT WE ARE SURROUNDED __ __STER_"
显然引号内第三个词是knowledge,则P->L, W->G:
THE PASSWORD IS THE SURNA_E O_ THE _AN WHO SAID "THE HIGEST KNOWLEDGE IS TO KNOW THAT WE ARE SURROUNDED __ __STER_"
卧槽，密文抄错了，highest应该是没问题的。
be surrounded by,所以GH应该是by，而EX这里对O_只剩of这个可能了。
THE PASSWORD IS THE SURNA_E OF THE _AN WHO SAID "THE HIGHEST KNOWLEDGE IS TO KNOW THAT WE ARE SURROUNDED BY _YSTERY"
显然最后一个词是mystery了，至此所有的单词都已经对应完全了，密文为：
the password is the surname of the man who said "the highest knowledge is to know that we are surrounded by mystery"
这句话是Albert Schweitzer说的，他的姓显然就是albert了
那么password就应当是：ALBERT

*/
int main()
{
    string ciphertext = "MAL TIRRUEZF CR MAL RKZYIOL EX MAL OIY UAE RICF \"MAL ACWLRM DYEUPLFWL CR ME DYEU MAIM UL IZL RKZZEKYFLF GH OHRMLZH\"";
    //caesar(ciphertext);
    frequency(ciphertext);
}