#include "lab4.h"
//when getline() and freopen() use at the same time, need to use cin.clear() to reset the cin!
vector<mydiary> diarylist;
void mydiary::setcontent(string str)
{
    content = str;
}

void mydiary::setdate(long data)
{
    date = data;
}

string mydiary::getcontent()
{
    return content;
}

long mydiary::getdate()
{
    return date;
}
//read the diary
void read_diary()
{
    long tempdate;
    string recorder = "";
    string tempcontent;
    mydiary tempdiary;
    freopen("./diary.txt", "r", stdin);//freopen to the diary text
    while (cin >> tempdate)
    {
        getline(cin, tempcontent);
        getline(cin, tempcontent);
        while (tempcontent != ".")
        {
            recorder += tempcontent + '\n';
            getline(cin, tempcontent);
        }
        tempdiary.setcontent(recorder);
        tempdiary.setdate(tempdate);
        diarylist.push_back(tempdiary);
        tempdate = 0;
        tempcontent = recorder = "";
    }
    cin.clear();//clear the cin
    fclose(stdin);
    freopen("CON", "r", stdin);//begin to cin from the console
}
//write the diary
void write_diary()
{
    //fstream fs("./diary.txt", fstream::out | ios_base::trunc);
    mydiary tempdiary;
    freopen("./diary.txt", "w", stdout);//begin to cout to the diary text
    int i = 0;
    for (i = 0; i < diarylist.size(); i++)
    {
        cout << diarylist[i].getdate() << endl;
        cout << diarylist[i].getcontent();
        cout << "." << endl;
    }
    fclose(stdout);//end freopen
}