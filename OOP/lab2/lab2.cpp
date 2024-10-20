// ATTENTION! the input end with EOF
// that means, after copy the data into the console, you need to input "\n" and "ctrl+Z" for Windows
// to describe EOF
// otherwise the program cannot stop.
// please read README.txt to know the file allocation
// note: in bits/stdc++.h, there is an operand named "count".
#include "lab2.h"
using namespace std;

void stu::getname(string tempname)
    {
        name = tempname;
    }
    
void stu::getexam(string exam)
    {
        privateexam.push_back(exam);
    }

void stu::getscore(int score)
    {
        privatescore.push_back(score);
    }

double stu::putaverage()
    {
        int i = 0;
        double sum = 0.0;
        for (i = 0; i < privatescore.size(); i++)
            sum += privatescore[i];
        return sum / privatescore.size();
    }

int stu::put_certain_course(string exam)
    {
        int i = 0;
        for (i = 0; i < privateexam.size(); i++)
        {
            if (privateexam[i] == exam)
                return privatescore[i];
        }
        return -1;
    }

string stu::putname()
    {
        return name;
    }

// update the examlist
void update_new_exam(string exam)
{
    int i = 0;
    int sign = 0;
    while (i < examlist.size())
    {
        if (examlist[i] == exam)
        {
            sign = 1;
            break;
        }
        else i++;
    }
    if (sign == 0)
    {
        examlist.push_back(exam);
    }
}
// output the average
void output_average(int* list, int counter, int* min, int* max, int exam)
{
    double sum = 0.0;
    int i = 0;
    int checkmin = list[0];
    int checkmax = list[0];
    for (i = 0; i < counter; i++)
        sum += list[i];
    sum /= counter;
    cout << sum << "\t";

    for (i = 0; i < counter; i++)
    {
        if (checkmin > list[i])
            checkmin = list[i];
        if (checkmax < list[i])
            checkmax = list[i];
    }
    min[exam] = checkmin;
    max[exam] = checkmax;
}
// the whole process of output
void output()
{
    int i = 0, j = 0, k = 0;
    int tempdata = 0;
    int *tempscore = (int*)malloc(sizeof(int) * (count_stu + 1));
    int *min = (int*)malloc(sizeof(int) * examlist.size());
    int *max = (int*)malloc(sizeof(int) * examlist.size());
    cout << "no\t" << "name\t";
    for (i = 0; i < examlist.size(); i++)
        cout << examlist[i] << "\t";
    cout << "average\n";

    for (i = 0; i < count_stu; i++)
    {
        cout << i + 1 << "\t";
        cout << student[i].putname() << "\t";
        for (j = 0; j < examlist.size(); j++)
        {
            if ((tempdata = student[i].put_certain_course(examlist[j])) != -1)
            {
                cout << tempdata;
            }

            cout << "\t";
        }
        cout << student[i].putaverage() << "\n";
    }

    cout << "\t" << "average\t";
    for (j = 0; j < examlist.size(); j++)
    {
        for (i = 0; i < count_stu; i++)
        {
            if ((tempdata = student[i].put_certain_course(examlist[j])) != -1)
            {
                tempscore[k] = tempdata;
                k++;
            }
        }
        output_average(tempscore, k, min, max, j);
        k = 0;
    }
    cout << "\n";

    cout << "\t" << "min\t";
    for (i = 0; i < examlist.size(); i++)
        cout << min[i] << "\t";
    cout << "\n";
    
    cout << "\t" << "max\t";
    for (i = 0; i < examlist.size(); i++)
        cout << max[i] << "\t";
}
// main program
int main()
{
    char tempc;
    string data;
    string tempread;
    data.clear();
    int i = 0;
    cout << "please choose the way to input:" << endl;
    cout << "copy data into console: input 1; directly read the file: input 2" << endl;
    cin >> i;
    if (i != 2)
    {
        while (getline(cin, tempread))// read till EOF
        {
            data += tempread;
            data += '\n';
        }
        data += '\0';
    }
    else if (i == 2)
    {
        cout << "choose file 1/2/3 to check the answer(just input 1 or 2 or 3)" << endl;
        cin >> i;
        if (i == 1)
        {
            fstream fp;
            fp.open("input1.txt", ios::in);
            while (getline(fp, tempread))
            {
                data += tempread;
                data += '\n';
            }
            data += '\0';
            fp.close();
        }
        else if (i == 2)
        {
            fstream fp;
            fp.open("input2.txt", ios::in);
            while (getline(fp, tempread))
            {
                data += tempread;
                data += '\n';
            }
            data += '\0';
            fp.close();
        }
        else if (i == 3)
        {
            fstream fp;
            fp.open("input3.txt", ios::in);
            while (getline(fp, tempread))
            {
                data += tempread;
                data += '\n';
            }
            data += '\0';
            fp.close();
        }
    }
    int reader = 0;
    int inputsign = 0;
    int tempscore = 0;
    string tempname;
    tempname.clear();
    // there is 3 states for input:
    // inputsign = 0: read the name of student
    // inputsign = 1: read the name of exam
    // inputsign = 2: read the score of exam
    while ((tempc = data[reader]) != '\0')
    {
        if (tempc != ' ' && tempc != '\n' && tempc != '\0')
        {
            if (inputsign != 2)
                tempname += tempc;
            else if (inputsign == 2 && ((tempc >= '0') || tempc <= '9'))
                tempscore = tempc - '0';
        }
        else if (tempc == ' ' && inputsign == 0)
        {
            student[count_stu].getname(tempname);
            tempname.clear();
            inputsign = 1;
        }
        else if (tempc == ' ' && inputsign == 1)
        {
            student[count_stu].getexam(tempname);
            update_new_exam(tempname);
            tempname.clear();
            inputsign = 2;
        }
        else if (inputsign == 2)
        {
            student[count_stu].getscore(tempscore);
            tempscore = 0;
            if (tempc == ' ')
                inputsign = 1;
            else if (tempc == '\n')
            {
                count_stu++;
                inputsign = 0;
            }
        }
        reader++;
    }
    output();
}