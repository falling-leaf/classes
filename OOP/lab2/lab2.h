#ifndef LAB2_H
#define LAB2_H
#include <bits/stdc++.h>
using namespace std;
class stu
{
private:
    string name;//name of student
    vector<string>privateexam;//examname of student
    vector<int>privatescore;//examscore of student

public:
    void getname(string tempname);//get the name of student
    void getexam(string exam);//get the examname of student
    void getscore(int score);//get the examscore of student
    double putaverage();//output the average of score
    int put_certain_course(string exam);//output the score of exam
    string putname();//output name
};
vector<string>examlist;//the whole exams
stu student[50000];
int count_stu = 0;
void update_new_exam(string exam);// update the examlist
void output_average(int* list, int counter, int* min, int* max, int exam);// output the average
void output();// the whole process of output
#endif