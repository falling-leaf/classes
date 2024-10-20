// #include <bits/stdc++.h>
// #include <windows.h>
// #include "lab4.h"
// using namespace std;

// void pdadd()
// {
//     mydiary tempdiary;//create a new element
//     string data = "";//temp data
//     long datenow;//temp date
//     bool sign = false;//if the new element has been inserted or not
//     int i = 0;
//     string temp;
//     getline(cin, temp);
//     while (temp != ".")
//     {
//         data += temp + "\n";
//         getline(cin, temp);
//     }
//     SYSTEMTIME timenow;//get present date
//     GetLocalTime(&timenow);
//     datenow = timenow.wYear * 10000 + timenow.wMonth * 100 + timenow.wDay;

//     tempdiary.setdate(datenow);
//     tempdiary.setcontent(data);
//     for (i = 0; i < diarylist.size(); i++)//insert the new element
//     {
//         if (diarylist[i].getdate() > datenow)
//         {
//             diarylist.insert(diarylist.begin() + i, tempdiary);//insert
//             sign = true;
//         }
//     }
//     if (!sign)
//         diarylist.push_back(tempdiary);
// }
// //list the date of the diaries
// void pdlist()
// {
//     int i = 0;
//     for (i = 0; i < diarylist.size(); i++)
//     {
//         cout << diarylist[i].getdate() << endl;
//     }
// }

// void pdlist(long start, long end)
// {
//     int i = 0;
//     bool sign = false;
//     if (start > end)
//         cout << "the start time is later than the end time!" << endl;
//     for (i = 0; i < diarylist.size(); i++)
//     {
//         if (diarylist[i].getdate() == start || diarylist[i].getdate() == end)
//             sign = true;
//         if (sign)
//         {
//             cout << diarylist[i].getdate() << endl;
//         }
//         if (diarylist[i].getdate() == end)
//             sign = false;
//     }
// }

// void pdshow()
// {
//     long target;
//     cin >> target;
//     int i = 0;
//     int diary_number = 1;
//     for (i = 0; i < diarylist.size(); i++)
//     {
//         if (diarylist[i].getdate() == target)
//         {
//             cout << "diary " << diary_number << " on this day:" << endl;
//             cout << diarylist[i].getcontent() << endl;
//             diary_number++;
//         }
//     }
//     if (diary_number == 1)
//     {
//         cout << "no diary in this date." << endl;
//     }
// }

// void pdremove()
// {
//     long target;
//     cin >> target;
//     int i = 0;
//     for (i = 0; i < diarylist.size(); i++)
//     {
//         if (diarylist[i].getdate() == target)
//         {
//             diarylist.erase(diarylist.begin() + i);
//             i = 0;
//         }
//     }
//     cout << "dairy in the date " << target << " has been removed." << endl;
// }

// int main()
// {
//     string opt;
//     long buffer;
//     while (1)
//     {
//         cin >> opt;
//         if (opt == "./pdadd")
//             pdadd();
//         else if (opt == "./pdlist")
//             pdlist();
//         else if (opt == "./pdshow")
//             pdshow();
//         else if (opt == "./pdremove")
//             pdremove();
//         else break;
//     }
// }
