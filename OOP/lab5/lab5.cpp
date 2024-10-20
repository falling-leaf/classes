#include "lab5.h"

//simplify the fraction
void Fraction::simplify()
{
    int i = 2;
    for (i = 2; i <= upper && i <= lower; i++)
    {
        if (upper % i == 0 && lower % i == 0)
        {
            upper /= i;
            lower /= i;
            i = 1;
        }
    }
}

//the default ctor
Fraction::Fraction()
{
    upper = 0;
    lower = 1;
}

//the set ctor at the beginning
Fraction::Fraction(int a, int b)
{
    Fraction c;
    if (b == 0)
    {
        cout << "the lower digit cannot be 0." << endl;
        upper = 0;
        lower = 1;
    }
    else
    {
        c.set_Fraction(a, b);
        c.simplify();
        upper = c.get_upper();
        lower = c.get_lower();
    }
}

//the set ctor to change the value
void Fraction::set_Fraction(int a, int b)
{
    upper = a;
    lower = b;
    return;
}

//the set ctor to change upper
void Fraction::set_upper(int a)
{
    upper = a;
    return;
}

//the set ctor to change lower
void Fraction::set_lower(int b)
{
    if (b == 0)
    {
        cout << "lower digit cannot be 0." << endl;
        return;
    }
    else lower = b;
    return;
}

//get the upper value
int Fraction::get_upper()
{
    return upper;
}

//get the lower value
int Fraction::get_lower()
{
    return lower;
}

//the copy ctor
void Fraction::copy_Fraction(Fraction a)
{
    upper = a.get_upper();
    lower = a.get_lower();
    return;
}

//add operation
Fraction Fraction::operator+(Fraction b)
{
    Fraction c;
    int lowermult, upperadd;
    lowermult = this->lower * b.lower;
    upperadd = this->upper * b.lower + this->lower * b.upper;
    c.set_Fraction(upperadd, lowermult);
    c.simplify();
    return c;
}

//sub operation
Fraction Fraction::operator-(Fraction b)
{
    Fraction c;
    int lowermult, upperadd;
    lowermult = this->lower * b.lower;
    upperadd = this->upper * b.lower - this->lower * b.upper;
    c.set_Fraction(upperadd, lowermult);
    c.simplify();
    return c;
}

//mult operation
Fraction Fraction::operator*(Fraction b)
{
    Fraction c;
    int lowertemp, uppertemp;
    lowertemp = this->lower * b.lower;
    uppertemp = this->upper * b.upper;
    c.set_Fraction(uppertemp, lowertemp);
    c.simplify();
    return c;
}

//devide operation
Fraction Fraction::operator/(Fraction b)
{
    Fraction c;
    int lowertemp, uppertemp;
    lowertemp = this->lower * b.upper;
    uppertemp = this->upper * b.lower;
    c.set_Fraction(uppertemp, lowertemp);
    c.simplify();
    return c;
}

//a < b
bool Fraction::operator<(Fraction b)
{
    int x, y;
    x = this->upper * b.lower;
    y = this->lower * b.upper;
    if (x < y)
        return true;
    else return false;
}

//a <= b
bool Fraction::operator<=(Fraction b)
{
    int x, y;
    x = this->upper * b.lower;
    y = this->lower * b.upper;
    if (x <= y)
        return true;
    else return false;
}

//a == b
bool Fraction::operator==(Fraction b)
{
    int x, y;
    x = this->upper * b.lower;
    y = this->lower * b.upper;
    if (x == y)
        return true;
    else return false;
}

//a >= b
bool Fraction::operator>=(Fraction b)
{
    int x, y;
    x = this->upper * b.lower;
    y = this->lower * b.upper;
    if (x >= y)
        return true;
    else return false;
}

//a > b
bool Fraction::operator>(Fraction b)
{
    int x, y;
    x = this->upper * b.lower;
    y = this->lower * b.upper;
    if (x > y)
        return true;
    else return false;
}

//a != b
bool Fraction::operator!=(Fraction b)
{
    int x, y;
    x = this->upper * b.lower;
    y = this->lower * b.upper;
    if (x != y)
        return true;
    else return false;
}

//to doulbe type
double Fraction::fraction_double()
{
    double a = upper;
    double b = lower;
    return a / b;
}

//to string
string Fraction::fraction_string(int index)
{
    string str = "";
    if (index != 1)
    {
        str = to_string(upper) + "/" + to_string(lower);
        return str;
    }
    else
    {
        double a = upper;
        double b = lower;
        a = a / b;
        str = to_string(a);
        return str;
    }
}

//to string for default
string Fraction::fraction_string()
{
    return (this->fraction_string(1));
}

//the conversion from string to fraction
Fraction Fraction::conversion(string str)
{
    Fraction c;
    int i = 0, j = 0, sum = 0;
    for (i = 0; str[i] != '.' && str[i] != '\0'; i++);
    if (str[i] == '\0')
    {
        cout << "this string can't be recognized as a fraction." << endl;
        return c;
    }
    for (i++; str[i] != '\0'; i++,j++);
    c.lower = pow(10, j);
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '.')
        {
            sum *= 10;
            sum += str[i] - '0';
        }
    }
    c.upper = sum;
    c.simplify();
    return c;
}

//the cin
istream &operator>>(istream &input, Fraction &D)
{
    input >> D.upper >> D.lower;
    return input;
}

//the cout
ostream &operator<<(ostream &output, Fraction &D)
{
    output << D.get_upper() << "/" << D.get_lower();
    return output;
}



int main()
{
    Fraction f1;//test default ctor
    Fraction f2(4, 7);//test infinite decimal string
    Fraction f3(1, 0);//test lower digit is 0(output notice)
    Fraction f4(3, 6);//test finite decimal string
    Fraction f;
    cin >> f;//test cin
    cout << f << endl;//test cout
    cout << f1.fraction_string(0) << endl;//output default 0/1

    f1.copy_Fraction(f2);//test copy ctor
    cout << f1.fraction_string(0) << endl;//output copy result 4/7
    cout << f4.fraction_string(0) << endl;//output f4 with simplification 1/2

//test the operator
    Fraction f5;
    f5 = f2 + f4;
    cout << f5.fraction_string(0) << endl;
    f5 = f2 - f4;
    cout << f5.fraction_string(0) << endl;
    f5 = f2 * f4;
    cout << f5.fraction_string(0) << endl;
    f5 = f2 / f4;
    cout << f5.fraction_string(0) << endl;

    if (f2 > f4)
        cout << "f2 > f4" << endl;
    if (f2 != f4)
        cout << "f2 != f4" << endl;

//test the conversion
    cout << f2.fraction_double() << endl;//output the double data of f2
    cout << f2.fraction_string() << endl;//output the string double of f2
    f5 = f5.conversion("999.999");
    cout << f5.fraction_string(0) << endl;//output the conversion result of f2
}