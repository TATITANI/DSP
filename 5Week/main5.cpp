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
    // 2 : x = 1.02 + 0.81j, y = -0.29 + 0.43j
    // ax + by = e
    // cx + dy = f
    // x = (ed-fb)/(ad-bc), y = (af-ce)/(ad-bc)
    Complex a(2, 1), b(3, -2), c(3, 0), d(-3, 5), e(2, 4), f(1, 1);

    Complex x = (e * d - f * b) / (a * d - b * c);
    Complex y = (a * f - c * e) / (a * d - b * c);
    x.Print("x");
    y.Print("y");
}

void SolveProblem3()
{
    cout << endl << "===problem3===" << endl;

    const int N = 8;
    const int m = 6, n = 6;
    Complex Vm(0, 0), Vn(0, 0), Vmn(0,0);
    for (int i = 0; i < N; i++)
    {
        Complex mi(cos(2 * PI * m * i / N) / sqrt(N), sin(2 * PI * m * i / N) / sqrt(N));
        Vm += mi;
        Complex ni(cos(2 * PI * n * i / N) / sqrt(N), sin(2 * PI * n * i / N) / sqrt(N));
        Vn += ni;
        Complex mni(cos(2 * PI * (m-n) * i / N), sin(2 * PI * (m-n) * i / N));
        Vmn += mni;
    }
    Vmn = Vmn/N;

    Vm.Print("Vm");
    (Vm * Vn).Print("result");
    Vmn.Print("Vmn");
}

void SolveProblem4()
{
    cout << endl
         << "===problem4===" << endl;
    // 3x^2 + 2x + 7 = 0;
    //  x = -0.333+1.4907j or x = -0.333 - 1.4907j
    float a = 3, b = 2, c = 7;
    float discriminant = b * b - 4 * a * c;

    bool plus = discriminant > 0;
    Complex x1, x2;
    x1 = Complex((-b + (plus ? sqrt(discriminant) : 0)), plus ? 0 : sqrt(-discriminant)) / (2 * a);
    x2 = Complex((-b - (plus ? sqrt(discriminant) : 0)), plus ? 0 : -sqrt(-discriminant)) / (2 * a);
    x1.Print("x1");
    x2.Print("x2");
}
