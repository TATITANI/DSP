#include "complex.h"

Complex Complex::operator+(const Complex &y)
{
    Complex c;
    c.re = re + y.re;
    c.im = im + y.im;
    return c;
}
Complex Complex::operator+=(const Complex &y)
{
    re += y.re;
    im += y.im;
}

Complex Complex::operator-(const Complex &y)
{
    Complex c;
    c.re = re - y.re;
    c.im = im - y.im;
    return c;
}

Complex Complex::operator*(const Complex &y)
{
    //(re+im*j)(re2+im2*j) = (re*re2-im*im2)+(re*im2+im*re2)
    Complex c;
    c.re = re * y.re - im * y.im;
    c.im = re * y.im + im * y.re;
    return c;
};

Complex Complex::operator/(const Complex &y)
{
    //(re+im*j)/(re2+im2*j) = (re*re2+im*im2)/(re2^2+im2^2) + (im*re2-re*im2)/(re2^2+im2^2)
    Complex c;
    c.re = (re * y.re + im * y.im) / (y.re * y.re + y.im * y.im);
    c.im = (-re * y.im + im * y.re) / (y.re * y.re + y.im * y.im);
    return c;
}

Complex Complex::operator/(const float &y)
{
    Complex c;
    c.re = re/y;
    c.im = im/y;
    return c;
}

void Complex::Print(string name)
{
    cout << name << " = " << re << (im>=0? " +" : " " )<< im << "j" << endl;
}
