#include <iostream>
#include "complex.h"
#include <math.h>
#define PI 3.141592

void SolveProblem2();
void SolveProblem3();
void SolveProblem4();

int main()
{
    cout.setf(ios::fixed);
    cout.precision(3);

    SolveProblem2();
    SolveProblem3();
    SolveProblem4();
    return 1;
}

void SolveProblem2()
{
    cout << endl
         << "===problem2===" << endl;
    // ax + by = e
    // cx + dy = f
    // x = (ed-fb)/(ad-bc), y = (af-ce)/(ad-bc)
    Complex a(2, 1), b(3, -2), c(3, 0), d(-3, 5), e(2, 4), f(1, 1);

    Complex x = (e * d - f * b) / (a * d - b * c);
    Complex y = (a * f - c * e) / (a * d - b * c);
    x.Print("x");
    y.Print("y");
}

void GenerateVmn(int N, int m, int n)
{
    Complex Vmn(0, 0);
    for (int i = 0; i < N; i++)
    {
        Complex mi(cos(2 * PI * m * i / N) / sqrt(N), sin(2 * PI * m * i / N) / sqrt(N));
        Complex ni(cos(2 * PI * n * i / N) / sqrt(N), -sin(2 * PI * n * i / N) / sqrt(N));
        Vmn += (mi * ni);
    }
    cout << "m = " << m << ", n = " << n << " => ";
    Vmn.Print("Vmn");
}

void SolveProblem3()
{
    cout << endl
         << "===problem3===" << endl;
    const int N = 8;
    for (int m = 0; m < N; m++)
    {
        for (int n = 0; n < N; n++)
        {
            GenerateVmn(N, m, n);
        }
        cout << endl;
    }
}

void SolveProblem4()
{
    cout << endl
         << "===problem4===" << endl;
    // 3x^2 + 2x + 7 = 0;
    float a = 3, b = 2, c = 7;
    float discriminant = b * b - 4 * a * c;

    bool plus = discriminant > 0;
    Complex x1, x2;
    x1 = Complex((-b + (plus ? sqrt(discriminant) : 0)), plus ? 0 : sqrt(-discriminant)) / (2 * a);
    x2 = Complex((-b - (plus ? sqrt(discriminant) : 0)), plus ? 0 : -sqrt(-discriminant)) / (2 * a);
    x1.Print("x1");
    x2.Print("x2");
}
