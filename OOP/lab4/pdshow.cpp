#include "lab4.h"
extern vector<mydiary> diarylist;
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "please input the date of diary correctly." << endl;
        return 0;
    }
    read_diary();
    long target = 0;
    int i = 0;
    string tempstr = argv[1];
    for (i = 0; i < tempstr.size(); i++)
    {
        target *= 10;
        target += tempstr[i] - '0';
    }
    int diary_number = 1;
    for (i = 0; i < diarylist.size(); i++)
    {
        if (diarylist[i].getdate() == target)
        {
            cout << "diary " << diary_number << " on this day:" << endl;
            cout << diarylist[i].getcontent() << endl;//show the diary
            diary_number++;
        }
    }
    if (diary_number == 1)
    {
        cout << "no diary in this date." << endl;
    }
    write_diary();
}