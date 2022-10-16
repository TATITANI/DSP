#include <iostream>
#include <string>
using namespace std;
class Complex
{
private:
    /* data */
public:
    double re, im;
    Complex() { re = im = 0; };
    Complex(double x, double y)
    {
        re = x;
        im = y;
    };
    Complex operator+ (const Complex &);
    Complex operator+= (const Complex &);
    Complex operator-(const Complex &);
    Complex operator*(const Complex &);
    Complex operator/(const Complex &);
    Complex operator/(const float &);
    void Print(string name);
};
