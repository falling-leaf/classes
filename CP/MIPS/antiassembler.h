#ifndef ANTIA_H
#define ANTIA_H
#include <bits/stdc++.h>
using namespace std;
class codeline_for_antia
{
private:
    bool is_RType = false;
    bool is_IType = false;
    bool is_JType = false;
    bool is_SpecR = false;
    bool is_jr = false;
    bool is_lui = false;
    bool is_lwsw = false;
    bool is_beq = false;
    bool err = false;
    string input;
    string output;
public:
    void clear_string();
    bool input_string();
    void simplify_string();
    bool cancel_process();
    char read_Type();
    void decode_operation();
    void decode_register_forR();
    void decode_opt_forI();
    void decode_register_forI();
    void decode_forJ();
    void decode_func();
    void output_string();
};

void initialize_register();
void start_reading();
void end_reading();
void main_process();
string read_register(string str, int start);
string read_shamt(string str, int start);
string read_imm(string str);

fstream fp_for_antia;
fstream fp_output;
string r_catalogue[32];

#endif