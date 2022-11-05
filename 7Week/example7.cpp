// #include <fstream>
// #include <iostream>
// #include <math.h>
// #include "complex.h"
// #include <complex>
// #include <iostream>
// using namespace std;

// #define PI 3.141592

// int main() {
//     int N = 1000;
//     int k0 = 200;

//     Complex *x = new Complex[N];
//     for (int n = 0; n < N; n++) {
//         x[n] = Complex(cos((2 * PI * k0 * n) / (double)N), 0);
//     }

//     // DFT
//     Complex *X = new Complex[N];
//     for (int k = 0; k < N; k++) {
//         for (int n = 0; n < N; n++) {
//             X[k] += x[n] * Complex(cos((-2 * PI * k * n) / (double)N),
//                                    sin((-2 * PI * k * n) / (double)N));
//         }

//     }

//     // IDFT
//     Complex *x_ = new Complex[N];
//     for (int n = 0; n < N; n++) {
//         for (int k = 0; k < N; k++) {
//             x_[n] += X[k] * Complex(cos((2 * PI * k * n) / (double)N),
//                                     sin((2 * PI * k * n) / (double)N));
//         }
//         x_[n] = x_[n] / (double)N;
//     }

//     ofstream OutFile_n, OutFile_xn, OutFile_xn_;
//     OutFile_n.open("n.txt");
//     OutFile_xn.open("x[n].txt");
//     OutFile_xn_.open("x_[n].txt");

//     for (int n = 0; n < 20; n++) {
//         OutFile_n << n << endl;
//         OutFile_xn << x[n].re << endl;
//         OutFile_xn_ << x_[n].re << endl;
//     }
    
//     return 1;
// }