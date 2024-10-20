#include <bits/stdc++.h>
#include <windows.h>
#include "lab4.h"
extern vector<mydiary> diarylist;
int main()
{
    string a;
    mydiary intempdiary;//create a new element
    string data = "";//temp data
    long datenow;//temp date
    bool sign = false;//if the new element has been inserted or not
    int i = 0;
    string temp;
    read_diary();
    getline(cin, temp);
    while (temp != "." && !cin.eof())
    {
        data += temp + "\n";
        getline(cin, temp);//read the content 
    }
    SYSTEMTIME timenow;//get present date
    GetLocalTime(&timenow);
    datenow = timenow.wYear * 10000 + timenow.wMonth * 100 + timenow.wDay;//use long int to store time

    intempdiary.setdate(datenow);//set date
    intempdiary.setcontent(data);//set content
    for (i = 0; i < diarylist.size(); i++)//insert the new element
    {
        if (diarylist[i].getdate() == datenow)//if the date has been in the diary, add the content.
        {
            diarylist[i].setcontent(diarylist[i].getcontent() + data);//add
            sign = true;
            break;
        }
        if (diarylist[i].getdate() > datenow)//if the date is in the middle of the diary, insert the diary.
        {
            diarylist.insert(diarylist.begin() + i, intempdiary);//insert
            sign = true;
            break;
        }
    }
    if (!sign)
        diarylist.push_back(intempdiary);
    cout << "the content has been written into the diary." << endl;
    write_diary();
}