#ifndef LAB4_H
#define LAB4_H

#include <bits/stdc++.h>
using namespace std;
class mydiary//the main data type
{
private:
    long date;//the date of diary
    string content;//the content of diary
public:
    void setdate(long data);
    void setcontent(string str);
    long getdate();
    string getcontent();
};

void read_diary();//read the diary
void write_diary();//write the diary

#endif