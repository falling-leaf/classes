/*note: multiple comments have been written in the header file,
 *which describe the meaning of all the global variables defined in the program.
 *to use the program, just input the expression.
 *there mustn't be any extra word in the expression, including space.
 *this program supports all the math functions.
 *for further information, please read "Testing Result" chapter in the report.
 */


#include <bits/stdc++.h>
#include "project 2.h"
using namespace std;

bool checksign(char ch, int level)//is an opt: true; isn't an opt: false
{
    int i = 0;
    if (level == 1)//the first level is +/-
    {
        if (ch == '+' || ch == '-')
            return true;
        else return false;
    }
    else if (level == 2)//the second level is *//
    {
        if (ch == '*' || ch == '/')
            return true;
        else return false;
    }
    else if (level == 3)//the third level is ^
    {
        if (ch == '^')
            return true;
        else return false;
    }
    else return false;
}

bool checksign(char ch)//is an opt: true; isn't an opt: false
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
        return true;
    return false;
}

bool checkbracket(int i, string str)//is in a bracket: true; isn't in a braket:false
{
    int m = i;
    int sign = 0;
    for (m = i; m >= 0; m--)//no matter pairs of brackets before m, as long as there is one more (, then it is in a bracket.
    {
        if (str[m] == '(')//if is '(', sign plus
            sign++;
        if (str[m] == ')')//if is ')', sign minus
            sign--;
    }
    if (sign > 0)
        return false;
    else return true;
}

bool check_new_operand(string str)//if is a new operand: true; isn't a new operand: false
{
    int i = 0;
    for (i = 0; i < operand_counter; i++)//visit all the operandlist
    {
        if (str == operandlist[i])
            return false;
    }
    return true;
}

void buildtree(string str, tree* branch)//build the expression tree with recursion
{
    int i = 0;
    int j = 0;
    int end = 0;//record the end of string
    int k = 0;
    int level = 1;//the precedence of opt
    int sign = 1;
    int sum = 0;
    int count_bracket = 0;
    bool isfunc = false;
    string str1;//store the left string
    string str2;//store the right string
    tree* templeft = (tree*)malloc(sizeof(tree));//left node
    tree* tempright = (tree*)malloc(sizeof(tree));//right node
    //FOR special function: level = 4
    while (level != 5)//check the sign
    {
        isfunc = false;
        i = 0;
        while (str[i] != '\0')
            i++;
        end = i;
        if (level != 4)//check the opt
        {
            while (!checksign(str[i], level) || !checkbracket(i, str))//from level 1 to 3, check the opt without bracket
            {
                i--;
                if (i == -1)
                    break;
            }
            if (str[i] == '-' && i == 0)//consider if the string is minus digit, which may lead minus to be sub.
            {
                for (j = 1; j < str.length(); j++)
                {
                    if (!(str[j] >= '0' && str[j] <= '9'))
                        break;
                }
                if (j == str.length())
                    i = -1;
            }
        }
        else if (level == 4)//if level is 4, then check the function
        {
            if (str.length() <= 4)
            {
                level++;
                continue;
            }
            else if (str.substr(0, 3) == "ln(")
            {
                branch->issign = true;
                branch->sign = 'l';
                str = str.substr(3, str.length() - 4);
                branch->left = NULL;
                branch->right = tempright;
                tempright->parent = branch;
                buildtree(str, tempright);//set the left of the tree
                return;
            }
            else if (str.substr(0, 4) == "sin(")
            {
                branch->issign = true;
                branch->sign = 's';
                str = str.substr(4, str.length() - 5);
                branch->left = NULL;
                branch->right = tempright;
                tempright->parent = branch;
                buildtree(str, tempright);//set the left of the tree
                return;
            }
            else if (str.substr(0, 4) == "cos(")
            {
                branch->issign = true;
                branch->sign = 'c';
                str = str.substr(4, str.length() - 5);
                branch->left = NULL;
                branch->right = tempright;
                tempright->parent = branch;
                buildtree(str, tempright);//set the left of the tree
                return;
            }
            else if (str.substr(0, 4) == "tan(")
            {
                branch->issign = true;
                branch->sign = 't';
                str = str.substr(4, str.length() - 5);
                branch->left = NULL;
                branch->right = tempright;
                tempright->parent = branch;
                buildtree(str, tempright);//set the left of the tree
                return;
            }
            else if (str.substr(0, 4) == "exp(")
            {
                branch->issign = true;
                branch->sign = 'e';
                str = str.substr(4, str.length() - 5);
                branch->left = NULL;
                branch->right = tempright;
                tempright->parent = branch;
                buildtree(str, tempright);//set the left of the tree
                return;
            }
            else
            {
                level++;
                continue;
            }
        }
        if (i != -1 && level != 0)//there is the needed sign node
        {
            branch->issign = true;
            branch->sign = str[i];
            str1 = str.substr(0, i);//left string update
            str2 = str.substr(i + 1, end - i);//right string update
            if (str[i - 1] == ')')//delete the widest bracket to decrease the level
            {
                k = i - 1;
                while (k >= 0)
                {
                    if (str[k] == '(')
                        count_bracket--;
                    if (str[k] == ')')
                        count_bracket++;
                    if (count_bracket == 0)
                        break;
                    k--;
                }
                if (k == 0 && str[k] == '(' && count_bracket == 0)
                {
                    str1 = str1.substr(1, i - 2);//delete the widest bracket
                }
                count_bracket = 0;
            }
            if (str[i + 1] == '(')//the same as the left string
            {
                k = i + 1;
                while (k <= end - 1)
                {
                    if (str[k] == '(')
                        count_bracket--;
                    if (str[k] == ')')
                        count_bracket++;
                    if (count_bracket == 0)
                        break;
                    k++;
                }
                if (k == end - 1 && str[k] == ')' && count_bracket == 0)
                {
                    str2 = str2.substr(1, end - i - 3);//delete the widest bracket
                }
                count_bracket = 0;
            }
            branch->left = templeft;
            branch->right = tempright;
            templeft->parent = branch;
            tempright->parent = branch;
            buildtree(str1, templeft);//set the left of the tree
            buildtree(str2, tempright);//set the right of the tree
            return;
        }
        else 
        {
            level++;
            continue;
        }
    }
    if (level == 5)//there is no sign in the string.
    {
        branch->issign = false;//issign is false
        if ((str[0] == '-') || (str[0] >= '0' && str[0] <= '9'))//the string represent a digit
        {
            i = 0;
            branch->isdigit = true;
            if (str[0] == '-')//the digit is minus
            {
                sign = -1;
                i++;
            }
            for (; str[i] != '\0'; i++)//change 'char' into 'int'
            {
                sum *= 10;
                sum += str[i] - '0';
            }
            sum *= sign;
            branch->digit = sum;//define the digit
        }
        else //the string represent an operand
        {
            branch->isdigit = false;
            for (i = 0; str[i] != '\0'; i++)//read the operand
                branch->operand[i] = str[i];
            branch->operand[i] = '\0';
            if (check_new_operand(str) && str != "e")//update the new operand
            {
                operandlist[operand_counter] = str;
                operand_counter++;
            }
        }
        branch->left = NULL;//the end of the node
        branch->right = NULL;//the end of the node
    }
}

void testtree(tree* temp)//set a test to check the data in tree with postorder
{
    if (temp->left != NULL)
        testtree(temp->left);
    if (temp->right != NULL)
        testtree(temp->right);
    if (temp->issign)
        cout << temp->sign;
    else if (temp->isdigit)
        cout << temp->digit;
    else cout << temp->operand;
}

void get_leaves(tree* br)//get all the leaves of the tree
{
    if (br != NULL)
    {
        if (br->left == NULL && br->right == NULL)
        {
            leaves[leaves_counter] = br;
            leaves_counter++;
        }
        get_leaves(br->left);
        get_leaves(br->right);
    }
}

bool check_trifunc(string str)//if str is triple function: true; otherwise is false.
{
    if (str != "sin" && str != "cos" && str != "tan" && str != "exp" && str != "pow" && str != "log")
        return false;
    else return true;
}

int judge_level(char ch)//judge the level of the sign
{
    int level = 5;
    if (ch == '+' || ch == '-')
        level = 1;
    if (ch == '*' || ch == '/')
        level = 2;
    if (ch == '^')
        level = 3;
    return level;
}

string take_bracket(string str)//delete the unused bracket
{
    string temp = str;
    bool bigbracket = false;
    bool appearsign = false;
    int i = 0, j = 0;
    int level = 5;
    int levelout = 0;
    for (i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == '(')
        {
            if (i >= 2)
            {
                if (temp.substr(i - 2, 2) == "ln")//if it is in ln function, not delete
                    continue;
            }
            if (i >= 3)
            {
                if (check_trifunc(temp.substr(i - 3, 3)))//if it is in math function, not delete
                {
                    continue;
                }
            }
            for (j = i + 1; temp[j] != ')'; j++)
            {
                if (temp[j] == '(')//ignore the second layer bracket
                {
                    for (; temp[j] != ')'; j++);
                }
                if (judge_level(temp[j]) < level)//update the level within the bracket
                {
                    appearsign = true;
                    level = judge_level(temp[j]);
                }
            }
            if (i != 0)
            {
                if (judge_level(temp[i - 1]) > levelout && judge_level(temp[i - 1]) != 5)//update the level out of the bracket
                    levelout = judge_level(temp[i - 1]);
            }
            j++;
            if (judge_level(temp[j]) > levelout && judge_level(temp[i - 1]) != 5)
                levelout = judge_level(temp[j]);
            if ((level >= levelout && appearsign) || !appearsign)//if precedence satisfies deleting the bracket
            {
                if (i > 0)
                {
                    if (temp[i - 1] == '/' && level == levelout && level == 2)
                        bigbracket = true;
                    if (checksign(temp[i - 1]) && checksign(temp[i + 1]))
                        bigbracket = true;
                }
                if (!bigbracket)
                {
                    temp = temp.substr(0, i) + temp.substr(i + 1, j - i - 2) + temp.substr(j, temp.length() - j);
                    i = -1;
                }
                bigbracket = false;
                appearsign = false;
            }
        }
        level = 5;
        levelout = 0;
    }
    return temp;
}

string calculate_digit(string str)//calculate the digit
{
    string temp = str;
    bool cal = false;
    int temp1 = 0, temp2 = 0;
    int i = 0, j = 0, k = 0;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (((str[i] == '*' || str[i] == '^') && str[i + 1] == '1') || (str[i] == '1' && str[i + 1] == '*'))
        {
            if (!(str[i + 2] >= '0' && str[i + 2] <= '9'))
            {
                str = str.substr(0, i) + str.substr(i + 2, str.length() - i - 2);
                i = 0;
            }
        }
        if (str[i] >= '0' && str[i] <= '9')//if is a digit
        {
            if (str[i - 1] == '/')
                continue;
            for (j = i; str[j] != '\0' && !checksign(str[j]) && str[j] != ')'; j++);
            if (!checksign(str[j]))
                continue;
            if (str[j] == '/')
                continue;
            j++;
            if (str[j] >= '0' && str[j] <= '9')//if the data after the sign is also a digit
                cal = true;
            for (k = j; str[k] >= '0' && str[k] <= '9'; k++);
            if (str[k] == '/')
                cal = false;
            if (cal)//calculate the digit
            {
                for (k = i; str[k] >= '0' && str[k] <= '9'; k++)
                    temp1 = temp1 * 10 + str[k] - '0';
                for (k = j; str[k] >= '0' && str[k] <= '9'; k++)
                    temp2 = temp2 * 10 + str[k] - '0';
                if (str[j - 1] == '+')
                    temp1 = temp1 + temp2;
                if (str[j - 1] == '-')
                    temp1 = temp1 - temp2;
                if (str[j - 1] == '*')
                    temp1 = temp1 * temp2;
                if (str[j - 1] == '^')
                    temp1 = pow(temp1, temp2); 
                str = str.substr(0, i) + to_string(temp1) + str.substr(k, temp.length() - k);//renew the string
                i = j = k = -1;
                cal = false;
                temp1 = temp2 = 0;
            }
        }
    }
    return str;
}

string delete_minus(string str)
{
    string temp = str;
    int i = 0;
    for (i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == '+' && temp[i + 1] == '-')
        {
            temp = temp.substr(0, i) + "-" + temp.substr(i + 2, temp.length() - i - 2);
            i = 0;
        }
    }
    return temp;
}

string simplify_result(string originstr)
{
    int i = 0, j = 0, k = 0;
    int match_bracket = 0;
    int data1 = 0;
    int data2 = 0;
    char opt;
    bool check_opt = false;
    bool break_loop = false;
    bool has_func = false;
    string temp = originstr;
    bool deletable = false;
    string checkfunc;
    string sub1;
    string sub2;

    temp = take_bracket(temp);
    temp = calculate_digit(temp);
    temp = take_bracket(temp);
    temp = calculate_digit(temp);
    temp = delete_minus(temp);
    return temp;
}

void output_nomial(tree* br)//output the operand/digit/function with this function
{
    if (br->isdigit)
        temp_polynomial += to_string(br->digit);//output the digit
    else temp_polynomial += br->operand;//output the operand
}

void output_tree(tree* br)//output the tree based on br, with the normal order
{
    temp_polynomial += "(";//protect bracket
    if (br->left != NULL)//give the left
    {
        if (br->left->issign == true)
            output_tree(br->left);
        else output_nomial(br->left);
    }
    if (br->issign)//give itself
    {
        if (br->sign == 'l')
            temp_polynomial += "ln(";
        else if (br->sign == 's')
            temp_polynomial += "sin(";
        else if (br->sign == 'c')
            temp_polynomial += "cos(";
        else if (br->sign == 't')
            temp_polynomial += "tan(";
        else if (br->sign == 'e')
            temp_polynomial += "e^(";
        else temp_polynomial += br->sign;
    }
    else output_nomial(br);
    if (br->right != NULL)//give the right
    {
        if (br->right->issign == true)
            output_tree(br->right);
        else output_nomial(br->right);
    }
    if (br->issign)
    {
        if (br->sign == 'l' || br->sign == 's' || br->sign == 'c' || br->sign == 't' || br->sign == 'e')
            temp_polynomial += ")";
    }
    temp_polynomial += ")";
}

void relist_operand()//relist operand with the lexigraphical order
{
    int i = 0;
    int j = 0;
    string temp;
    for (i = operand_counter - 1; i >= 0; i--)
    {
        for (j = i; j < operand_counter - 1; j++)
        {
            if (operandlist[j] > operandlist[j + 1])
            {
                temp = operandlist[j];
                operandlist[j] = operandlist[j + 1];
                operandlist[j + 1] = temp;
            }
        }
    }
}

void output_result()//cope the whole tree and output the result
{
    int i = 0, j = 0;
    int multi_multiple = 0;
    bool hasvalue = false;
    string target_operand;
    string tempstr;
    string partial_result = "";
    tree* elementbr;
    tree* storage;
    tree* temp;
    if (operand_counter == 0)
    {
        cout << "there is no variable in the expression!" << endl;
        return;
    }
    for (j = 0; j < operand_counter; j++)
    {
        target_operand = operandlist[j];//output the target_operand
        cout << target_operand << ": ";
        for (i = 0; i < leaves_counter; i++)
        {
            storage = elementbr = leaves[i];
            if (elementbr->operand != target_operand)//not the target operand, ignore
                continue;
            while (elementbr != root)
            {
                storage = elementbr;
                elementbr = elementbr->parent;
                if (elementbr->sign == 'l')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                        partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    partial_result += "1/";
                    temp_polynomial.clear();
                    output_tree(elementbr->right);
                    partial_result += temp_polynomial;
                }
                else if (elementbr->sign == 's')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                        partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    partial_result += "cos(";
                    temp_polynomial.clear();
                    output_tree(elementbr->right);
                    partial_result += temp_polynomial; 
                    partial_result += ")";
                }
                else if (elementbr->sign == 'c')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                        partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    partial_result += "(-sin";
                    temp_polynomial.clear();
                    output_tree(elementbr->right);
                    partial_result += temp_polynomial; 
                    partial_result += ")";
                }
                else if (elementbr->sign == 't')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                        partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    partial_result += "1/((cos";
                    temp_polynomial.clear();
                    output_tree(elementbr->right);
                    partial_result += temp_polynomial; 
                    partial_result += ")^2)";
                }
                else if (elementbr->sign == 'e')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                        partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    temp_polynomial.clear();
                    output_tree(elementbr->right); 
                    partial_result += "e^";
                    partial_result += temp_polynomial;
                }
                else if (elementbr->sign == '+')
                {
                    continue;
                }
                else if (elementbr->sign == '*')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                        partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    if (elementbr->left == storage)
                        temp = elementbr->right;
                    if (elementbr->right == storage)
                        temp = elementbr->left;
                    temp_polynomial.clear();
                    output_tree(temp);
                    partial_result += temp_polynomial;   
                }
                else if (elementbr->sign == '/')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                    partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    if (elementbr->left == storage)
                    {
                        temp_polynomial.clear();
                        output_tree(elementbr->right);
                        partial_result += "1/" + temp_polynomial;
                    }
                    else if (elementbr->right == storage)
                    {
                        temp_polynomial.clear();
                        output_tree(elementbr->left);
                        partial_result += "(-" + temp_polynomial + ")";
                        temp_polynomial.clear();
                        output_tree(elementbr->right);
                        partial_result += "/(" + temp_polynomial + "^2)";
                    }
                }
                else if (elementbr->sign == '^')
                {
                    hasvalue = true;
                    if (multi_multiple == 1)
                    partial_result += '*';
                    if (multi_multiple == 0)
                        multi_multiple = 1;
                    if (elementbr->left == storage)
                    {
                        temp_polynomial.clear();
                        output_tree(elementbr->right);
                        partial_result = partial_result + temp_polynomial + '*';
                        temp_polynomial.clear();
                        output_tree(elementbr->left);
                        partial_result = partial_result + temp_polynomial + '^';
                        temp_polynomial.clear();
                        output_tree(elementbr->right);
                        partial_result = partial_result + '(' + temp_polynomial + "-1" + ')';
                    }
                    else if (elementbr->right == storage)
                    {
                        temp_polynomial.clear();
                        output_tree(elementbr->left);
                        tempstr = temp_polynomial;
                        partial_result += temp_polynomial + '^';
                        temp_polynomial.clear();
                        output_tree(elementbr->right);
                        partial_result += temp_polynomial;
                        partial_result = partial_result + "*ln" + '(' + tempstr + ')';
                    }

                }
            }
            if (hasvalue == false)
                partial_result += "1";
            partial_result += "+";
            multi_multiple = 0;
            hasvalue = false;
        }
        partial_result = partial_result.substr(0, partial_result.length() - 1);
        partial_result = simplify_result(partial_result);
        cout << partial_result << endl;
        hasvalue = false;
        partial_result.clear();
        multi_multiple = 0;
    }
}

void cope_pow()//cope with the function pow(a,b)
{
    int i = 0;
    int j = 0;
    int temp = 0;
    int count_bracket = 0;
    string tempa;
    string tempb;
    if (dataline.length() <= 5)
        return;
    for (i = 0; i < dataline.length() - 4; i++)
    {
        if (dataline.substr(i, 4) == "pow(")
        {
            for (j = i + 4; dataline[j] != ','; j++)
            {
        
            }
            tempa = dataline.substr(i + 4, j - i - 4);
            j++;
            temp = j;
            for (; count_bracket != -1; j++)
            {
                if (dataline[j] == '(')
                    count_bracket++;
                if (dataline[j] == ')')
                    count_bracket--;
            }
            j--;
            tempb = dataline.substr(temp, j - temp);
            dataline = dataline.substr(0, i) + "(" + tempa + ")^(" + tempb + ")" + dataline.substr(j + 1, dataline.length() - j - 1); 
            i = j = 0;
            count_bracket = 0;
        }
    }
}

void cope_log()//cope with the function log(a,b)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int count_bracket = 0;
    string tempa;
    string tempb;
    if (dataline.length() <= 5)
        return;
    for (i = 0; i < dataline.length() - 5; i++)
    {
        if (dataline.substr(i, 3) == "log")
        {
            j = i + 4;
            for (k = j; dataline[k] != ','; k++)
            {

            }
            tempa = dataline.substr(j, k - j);
            j = k + 1;
            for (k = j; count_bracket != -1; k++)
            {
                if (dataline[k] == '(')
                    count_bracket++;
                if (dataline[k] == ')')
                    count_bracket--;
            }
            tempb = dataline.substr(j, k - j - 1);
            dataline = dataline.substr(0, i) + "((ln(" + tempb + "))/(ln(" + tempa + ")))" + dataline.substr(k, dataline.length() - k);
            i = 0;
            count_bracket = 0;
        }
    }
}

void cope_minus()
{
    int i = 0;
    int j = 0;
    bool need_cope = false;
    if (dataline.length() < 2)
        return;
    for (i = 0; i < dataline.length() - 1; i++)
    {
        if(dataline[i] == '-')
        {
            if (i != 0)
            {
                if (dataline[i - 1] == '(')
                    need_cope = true;
            }
            if (i == 0)
                need_cope = true;
            if (dataline[i + 1] >= '0' && dataline[i + 1] <= '9')
                need_cope = false;
            if (need_cope)
            {
                dataline = dataline.substr(0, i) + "(-1)*" + dataline.substr(i + 1, dataline.length() - i - 1);
                i = 0;
            }
        }
    }
}

int main()
{
    cout << "please input the expression:" << endl;
    getline(cin, dataline);
    cope_pow();
    cope_log();
    cope_minus();
    dataline = take_bracket(dataline);
    buildtree(dataline, root);
    //testtree(root);

    get_leaves(root);
    relist_operand();
    output_result();
}