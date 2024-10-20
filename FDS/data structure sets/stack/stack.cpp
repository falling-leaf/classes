//关于栈:FILO
//本程序中栈有dummy head
#include <bits/stdc++.h>
using namespace std;
struct Stack
{
    int data;
    Stack* next;
};

Stack* onestack = new Stack;
Stack* stackpt = onestack;

void onepop()
{

}

void onepush()
{
    int s;
    cin >> s;
    Stack* tempstack = (Stack*)malloc(sizeof(Stack));
    tempstack->data = s;
    tempstack->next = NULL;
    stackpt->next = tempstack;
    stackpt = tempstack;
}

int main()
{

}