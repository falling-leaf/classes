#ifndef LAB5_H
#define LAB5_H
#include <bits/stdc++.h>
using namespace std;
class Fraction
{
private: 
    int upper;//the upper digit
    int lower;//the lower digit
public:
    Fraction();//the initialization of fraction: set 0/1 at first
    Fraction(int a, int b);//set the value by the user, the result is a/b
    friend istream &operator>>(istream &input, Fraction &D);
    friend ostream &operator<<(istream &output, Fraction &D);
    Fraction operator+(Fraction b);//a + b
    Fraction operator-(Fraction b);//a - b
    Fraction operator*(Fraction b);//a * b
    Fraction operator/(Fraction b);//a / b
    bool operator<(Fraction b);//a < b
    bool operator<=(Fraction b);//a <= b
    bool operator==(Fraction b);//a == b
    bool operator!=(Fraction b);//a != b
    bool operator>=(Fraction b);//a >= b
    bool operator>(Fraction b);//a > b
    int get_upper();//get the upper digit
    int get_lower();//get the lower digit
    void simplify();//simplify the fraction
    void set_Fraction(int a, int b);//set the value of fraction
    void set_upper(int a);//set the upper digit of fraction
    void set_lower(int a);//set the lower digit of fraction
    void copy_Fraction(Fraction a);//copy the fraction a
    double fraction_double();//return the double data of the fraction
    string fraction_string();//return the string of fraction default
    string fraction_string(int index);//index = 1 return the double string, index = 0 return the fraction string
    Fraction conversion(string str);//convert the string to fraction

};
#endif