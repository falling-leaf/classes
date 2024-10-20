#ifndef PROJECT_H
#define PROJECT_H
#include <bits/stdc++.h>
using namespace std;

struct tree//the tree of expression
{
    int digit;//the digit of the node
    char operand[20];//the operand of the node
    char sign;//the sign of the node
    bool isdigit;//whether the data in the node is a digit
    bool issign;//whether the data in the node is a sign

    tree* left;//the left children of the node
    tree* right;//the right children of the node
    tree* parent;//the parent of the node
};

tree* root = (tree*)malloc(sizeof(tree));//the root of the expression tree
string dataline;//the dataline input

string operandlist[50];//the list of variables
int operand_counter = 0;//the counter of variables
tree* leaves[50];//the leaves of the tree
int leaves_counter = 0;//the number of leaves
string temp_polynomial = "";//the temp expression output

void buildtree(string str, tree* branch);//build the expression tree
bool checksign(char ch, int level);//check the sign with certain level
bool checksign(char ch);//check the sign
bool checkbracket(int i, string str);//check str[i] in a bracket or not
bool check_new_operand(string str);//check str is a new variable or not

void relist_operand();//list the variables in the correct order

void get_leaves(tree* br);//get all the leaves in the tree
void output_tree(tree* br);//output the tree with the root br
void output_nomial(tree* br);//output the nomial with the root br
void output_result();//output the result

bool check_trifunc(string str);//check if str is triple-word math function
void cope_pow();//cope with pow(a,b)
void cope_log();//cope with log(a,b)
void cope_minus();//cope with minus
string simplify_result(string originstr);//simplify the result
string take_bracket(string str);//take off the bracket
string calculate_digit(string str);//calculate the digit
string delete_minus(string str);//delete unused minus sign
#endif