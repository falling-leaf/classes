//write at the front:
//this program set EXAM and STUNUM to make it be used more widely,
//the default value is: EXAM = 3, STUNUM = 10.
//you can change the value of them to change the number of exams/students,
//(it is also for preparing Lab2 :D

//the whole program use "stu" class to load the data,
//and is divided into "input" "processing" "output" three parts to realize the result.

//(if the name is too long, the structure of output may be destroyed)

#include <iostream>
using namespace std;
#define EXAM 3
#define STUNUM 10

class stu   //use "stu" class to represent the information of every student
{
public:
    int no;         //No. of the student
    string name;    //name of the student
    int score[EXAM];   //3 scores of the student
    float average()
    {
        int i = 0;
        float sum = 0.0;
        while (i < EXAM)
        {
            sum += score[i];
            i++;
        }
        return sum / EXAM;  //calculate the average
    }
};

stu student[STUNUM];    //the main data in the whole program

int min(int count)      //calculate the minimum of the score
{
    int i = 0;
    int mini = student[i].score[count];
    while (i < STUNUM)
    {
        if (mini > student[i].score[count])
            mini = student[i].score[count];
        i++;
    }
    return mini;
}

int max(int count)      //calculate the maximum of the score
{
    int i = 0;
    int maxi = student[i].score[count];
    while (i < STUNUM)
    {
        if (maxi < student[i].score[count])
            maxi = student[i].score[count];
        i++;
    }
    return maxi;
}

float ave(int count)    //calculate the average of the score
{
    int i = 0;
    float sum = 0;
    while (i < STUNUM)
    {
        sum += student[i].score[count];
        i++;
    }
    sum /= STUNUM;
    return sum;
}

int main()
{
    //the input part(initializing the classes):
    int count = 0;
    int i = 0;
    int minimum[EXAM];
    int maximum[EXAM];
    float averageScore[EXAM];
    cout << "please input information like \"name score1 score2 score3...\"" << endl;
    while (count < STUNUM)
    {
        i = 0;
        student[count].no = count + 1;
        cin >> student[count].name;
        while (i < EXAM)  
        {
            cin >> student[count].score[i];
            i++;
        }
        count++;
    }

    //the processing part:
    count = 0;
    while (count < EXAM)
    {
        minimum[count] = min(count);
        maximum[count] = max(count);
        averageScore[count] = ave(count);
        count++;
    }

    //the output part:
        //the first line
    cout << "no" << "\t" << "name" << "\t";
    i = 1;
    while (i < EXAM + 1)
    {
        cout << "score" << i << "\t";
        i++;
    }
    cout << "average" << endl;
        //the data lines
    count = 0;
    while (count < STUNUM)
    {
        i = 0;
        cout << student[count].no << "\t" << student[count].name << "\t" ;
        while (i < EXAM)
        {
            cout << student[count].score[i] << "\t";
            i++;
        }
        cout << student[count].average() << endl;
        count++;
    }
        //the analysis lines
    cout << "\t" << "average" << "\t";
    i = 0;
    while (i < EXAM)
    {
        cout << averageScore[i] << "\t";
        i++;
    }
    cout << endl;
    cout << "\t" << "min" << "\t";
    i = 0;
    while (i < EXAM)
    {
        cout << minimum[i] << "\t";
        i++;
    }
    cout << endl;
    cout << "\t" << "max" << "\t";
    i = 0;
    while (i < EXAM)
    {
        cout << maximum[i] << "\t";
        i++;
    }
    cout << endl;
}