#include <bits/stdc++.h>
#include "antiassembler.h"
using namespace std;
//ATTENTION! the turn of rs, rt, rd need to change!
//note: use complement number or not?
//note: temporarily, suppose imm is complement number, shamt and address not use.

//update: clear_string renew
//update: beq/bne renew
//update: space only



//initialize the register catalogue
void initialize_register()
{
    int i = 0, j = 0;
    r_catalogue[0] = "$zero";
    r_catalogue[1] = "$at";
    r_catalogue[2] = "$v0";
    r_catalogue[3] = "$v1";
    for (i = 4; i < 8; i++)
    {
        j = i - 4;
        r_catalogue[i] = "$a" + to_string(j);
    }
    for (i = 8; i < 16; i++)
    {
        j = i - 8;
        r_catalogue[i] = "$t" + to_string(j);
    }
    for (i = 16; i < 24; i++)
    {
        j = i - 16;
        r_catalogue[i] = "$s" + to_string(j);
    }
    r_catalogue[24] = "$t8";
    r_catalogue[25] = "$t9";
    r_catalogue[26] = "$k0";
    r_catalogue[27] = "$k1";
    r_catalogue[28] = "$gp";
    r_catalogue[29] = "$sp";
    r_catalogue[30] = "$fp";
    r_catalogue[31] = "$ra";
}

//start reading
void start_reading()
{
    fp_for_antia.open("testdata.txt", ios::in);
    if (!fp_for_antia)
        exit(1);
}

//end reading
void end_reading()
{
    fp_for_antia.close();
}

//clear the content of string
void codeline_for_antia::clear_string()
{
    input.clear();
    is_RType = false;
    is_IType = false;
    is_JType = false;
    is_SpecR = false;
    is_jr = false;
    is_lui = false;
    is_lwsw = false;
    is_beq = false;
    output.clear();
}

//input the string
bool codeline_for_antia::input_string()
{
    string cope_endl;
    getline(fp_for_antia, input);
    if (input != "")
        return true;
    else return false;
}

//output the string
void codeline_for_antia::output_string()
{
    fp_output.open("result.txt", ios::app);
    if (!fp_output)
        exit(1);
    fp_output << output << endl;
    fp_output.close();
}

//check the type of operation
void codeline_for_antia::decode_operation()
{
    string op = input.substr(0, 6);
    if (op == "000000")
        is_RType = true;
    else if (op == "000010" || op == "000011")
        is_JType = true;
    else is_IType = true;
}

//for R type, check the func code to confirm the operation
void codeline_for_antia::decode_func()
{
    string func = input.substr(26, 6);
    if (func == "100000")
        output += "add ";
    if (func == "100001")
        output += "addu ";
    if (func == "100010")
        output += "sub ";
    if (func == "100011")
        output += "subu ";
    if (func == "100100")
        output += "and ";
    if (func == "100101")
        output += "or ";
    if (func == "100110")
        output += "xor ";
    if (func == "100111")
        output += "nor ";
    if (func == "101010")
        output += "slt ";
    if (func == "101011")
        output += "sltu ";
    if (func == "000000")
    {
        output += "sll ";
        is_SpecR = true;
    }
    if (func == "000010")
    {
        output += "srl ";
        is_SpecR = true;
    }
    if (func == "000011")
    {
        output += "sra ";
        is_SpecR = true;
    }
    if (func == "000100")
        output += "sllv ";
    if (func == "000110")
        output += "srlv ";
    if (func == "000111")
        output += "srav ";
    if (func == "001000")
    {
        output += "jr ";
        is_jr = true;
    } 

}

//convert the string into exact register
string read_register(string str, int start)
{
    int i = 0;
    int sum = 0;
    string bitstr = str.substr(start, 5);
    for (i = 0; i < 5; i++)
    {
        sum *= 2;
        sum += bitstr[i] - '0';
    }
    return r_catalogue[sum];
}

//convert the string into exact shamt
string read_shamt(string str, int start)
{
    int i = 0;
    int sum = 0;
    string bitstr = str.substr(start, 5);
    for (i = 0; i < 5; i++)
    {
        sum *= 2;
        sum += bitstr[i] - '0';
    }
    return to_string(sum);
}

//for R type, use this model to convert input into output
void codeline_for_antia::decode_register_forR()
{
    int i = 0;
    int sum = 0;
    if (is_SpecR)
    {
        output += read_register(input, 16);
        output += ", ";
        output += read_register(input, 11);
        output += ", ";
        output += read_shamt(input, 21);
    }
    else if (is_jr)
    {
        output += read_register(input, 6);
    }
    else 
    {
        output += read_register(input, 16);
        output += ", ";
        output += read_register(input, 6);
        output += ", ";
        output += read_register(input, 11);
    }
}

//for I type, use this model to convert input into output
void codeline_for_antia::decode_opt_forI()
{
    string op = input.substr(0, 6);
    if (op == "001000")
        output += "addi ";
    if (op == "001001")
        output += "addiu ";
    if (op == "001100")
        output += "andi ";
    if (op == "001101")
        output += "ori ";
    if (op == "001110")
        output += "xori ";
    if (op == "001111")
    {
        output += "lui ";
        is_lui = true;
    }
    if (op == "100011")
    {
        output += "lw ";
        is_lwsw = true;
    }
    if (op == "101011")
    {
        output += "sw ";
        is_lwsw = true;
    }
    if (op == "000100")
    {
        output += "beq ";
        is_beq = true;
    }
    if (op == "000101")
    {
        output += "bne ";
        is_beq = true;
    }
    if (op == "001010")
        output += "slti ";
    if (op == "001011")
        output += "sltiu ";
}

void codeline_for_antia::simplify_string()
{
    int i = 0;
    if (input == "\n")
    {
        err = true;
        return;
    }
    
    for (i = 0; i < input.size(); i++)
    {
        if (input[i] == ' ')
        {
            input = input.substr(0, i) + input.substr(i + 1, input.size() - i - 1);
            i = 0;
        }
    }
}

//read immediate number
string read_imm(string str)
{
    string bitstr = str.substr(17, 15);
    int sum = 0;
    int i = 0;
    for (i = 0; i < 15; i++)
    {
        sum *= 2;
        sum += bitstr[i] - '0';
    }
    if (str[16] == '1')
    {
        sum -= 32768;
        sum = -sum;
        return "-" + to_string(sum);
    }
    else return to_string(sum);
}

//decode the register
void codeline_for_antia::decode_register_forI()
{
    if (is_lui)
    {
        output += read_register(input, 11);
        output += ", ";
        output += read_imm(input);
    }
    else if (is_lwsw)
    {
        output += read_register(input, 11);
        output += ", ";
        output += read_imm(input);
        output += "(";
        output += read_register(input, 6);
        output += ")";
    }
    else if (is_beq)
    {
        output += read_register(input, 6);
        output += ", ";
        output += read_register(input, 11);
        output += ", ";
        output += read_imm(input);
    }
    else 
    {
        output += read_register(input, 11);
        output += ", ";
        output += read_register(input, 6);
        output += ", ";
        output += read_imm(input);
    }
}

//for J type, use this model to convert input into output
void codeline_for_antia::decode_forJ()
{
    string op = input.substr(0, 6);
    string address = input.substr(6, 26);
    int i = 0;
    int sum = 0;
    if (op == "000010")
        output += "j ";
    if (op == "000011")
        output += "jal ";
    for (i = 0; i < 26; i++)
    {
        sum *= 2;
        sum += address[i] - '0';
    }
    output += to_string(sum);
}

//read the type of string
char codeline_for_antia::read_Type()
{
    if (is_RType)
        return 'R';
    else if (is_IType)
        return 'I';
    else if (is_JType)
        return 'J';
    else return 'N';
}

//*change it
//void main_process()

bool codeline_for_antia::cancel_process()
{
    if (input == "\n")
        return true;
    else return false;
}

int main()
{
    codeline_for_antia codeline;
    initialize_register();
    start_reading();
    cout << "start processing..." << endl;
    while (1)
    {
        codeline.clear_string();
        if (!codeline.input_string())
            break;
        if (codeline.cancel_process())
            continue;
        codeline.simplify_string();
        codeline.decode_operation();
        if (codeline.read_Type() == 'R')
        {
            codeline.decode_func();
            codeline.decode_register_forR();
            codeline.output_string();                
        }
        else if (codeline.read_Type() == 'I')
        {
            codeline.decode_opt_forI();
            codeline.decode_register_forI();
            codeline.output_string();
        }
        else if (codeline.read_Type() == 'J')
        {
            codeline.decode_forJ();
            codeline.output_string();
        }
    }

    end_reading();
}