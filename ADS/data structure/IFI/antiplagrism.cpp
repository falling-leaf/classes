//在此题中，只需要考虑单词出现的次数，不需要考虑出现的位置
# include <bits/stdc++.h>
using namespace std;
typedef map<string, int> artindex;
typedef map<string, artindex> allindex;
allindex dict;
//整个数据用<题目-表>的结构
//表用<单词-出现次数>的结构
//顺带一提，如果是正常的IFI，那么还应该再套一个表
//再顺带一提，这个数据结构是一维数组，在查询时时间会很长

//不区分大小写
string copecapital(string content){
    for (int j = 0; content[j] != '\0'; j++){
        if (content[j] >= 'A' && content[j] <= 'Z')
            content[j] -= ('A' - 'a');
            }
    return content;
}

//处理部分stemming
string copestem(string content){
    int i = content.size() - 2;
    if (i < 0)
        return content;
    if (content[i] == 'e'){
        if (i == 0 || content[i - 1] != 'i'){
            if (content[i + 1] == 's' || content[i + 1] == 'd')
                return content.substr(0, content.size() - 2);
        }
    }
    i--;
    if (i < 0)
        return content;
    if (content[i] == 'i'){
        if (content[i + 1] == 'n' && content[i + 2] == 'g')
            return content.substr(0, content.size() - 3);
        if (content[i + 1] == 'e' && content[i + 2] == 's')
            return content.substr(0, content.size() - 3);
    }
    return content;
}


void insert(){
    int n;
    bool newterm = true;
    string name, content;
    artindex* article;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        article = new artindex;
        cin >> name;
        cin >>content;
        while (content != "#")
        {
            content = copecapital(content);
            content = copestem(content);
            newterm = true;
            for (auto it = (*article).begin(); it != (*article).end(); it++)
            {
                if (it->first == content)
                {
                    it->second++;
                    newterm = false;
                    break;
                }
            }
            if (newterm)
                (*article).insert(make_pair(content, 1));
            cin >> content;
        }
        dict.insert(make_pair(name, (*article)));
    }
    // for (auto it = dict["A02"].begin(); it != dict["A02"].end(); it++)
    //     cout << it->first << it->second;
}

//计算一个文本的模
double calmodel(artindex target)
{
    double sum = 0.0;
    for (auto it = target.begin(); it != target.end(); it++){
        sum += (it->second) * (it->second);
    }
    return sqrt(sum);
}

void copequery()
{
    int n;
    double sum;
    string art1, art2;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        sum = 0.0;
        cin >> art1 >> art2;
        for (auto it = dict[art1].begin(); it != dict[art1].end(); it++){
            for (auto jt = dict[art2].begin(); jt != dict[art2].end(); jt++){
                if (it->first == jt->first){
                    sum += it->second * jt->second;
                }
            }
        }
        sum = sum / calmodel(dict[art1]) / calmodel(dict[art2]);
        cout << "Case " << i + 1 << ": " << setprecision(3) << acos(sum) << endl;
    }
}


int main(){
    insert();
    copequery();
}