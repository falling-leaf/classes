#include "lab4.h"
extern vector<mydiary> diarylist;
int main(int argc, char* argv[])
{
    if (argc != 2)//there is not a date.
    {
        cout << "please input the date of diary correctly." << endl;
        return 0;
    }
    read_diary();
    long target = 0;
    string tempstr = argv[1];
    int i = 0;
    bool sign = false;
    for (i = 0; i < tempstr.size(); i++)
    {
        target *= 10;
        target += tempstr[i] - '0';
    }
    for (i = 0; i < diarylist.size(); i++)
    {
        if (diarylist[i].getdate() == target)
        {
            diarylist.erase(diarylist.begin() + i);//remove the data
            i = 0;
            sign = true;
        }
    }
    if (sign)
        cout << "diary in the date " << target << " has been removed." << endl;
    else cout << "not found any diary with the date of input." << endl;
    write_diary();
}