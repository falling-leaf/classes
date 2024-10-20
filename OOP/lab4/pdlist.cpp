#include "lab4.h"
extern vector<mydiary> diarylist;

long changedec(string str)//change ASCII to int
{
    long sum = 0;
    int i = 0;
    for (i = 0; i < str.size(); i++)
    {
        sum *= 10;
        sum += str[i] - '0';
    }
    return sum;
}
int main(int argc, char* argv[])
{
    read_diary();//read the diary
    int i = 0;
    string startstr, endstr;
    long start, end;
    if (argc == 1)//if not have 
    {
        for (i = 0; i < diarylist.size(); i++)
        {
            cout << diarylist[i].getdate() << endl;
        }
    }
    else {
        startstr = argv[1];
        endstr = argv[2];
        start = changedec(startstr);
        end = changedec(endstr);
        bool sign = false;
        if (start > end)
            cout << "the start time is later than the end time!" << endl;
        for (i = 0; i < diarylist.size(); i++)
        {
            if (diarylist[i].getdate() >= start && diarylist[i].getdate() <= end)
                cout << diarylist[i].getdate() << endl;
        }
    }
    write_diary();
}