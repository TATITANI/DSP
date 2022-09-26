#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#define PI 3.141592

int matrixMultiplication(float *a, float *b, float *c, int m, int n, int p);
float GetAlpha(int n);
float degToRad(float deg);
void swapRow(int m, int n, float *mA, float *mAd, int ii, int jj);
void swapCol(int m, int n, float *mA, float *mAd, int ii, int jj);
void PrintMatrix(float *a, int m, int n);

int main()
{
    float *A, *B, *C;
    int M, N, P;
    M = 8, N = 8, P = 8;
    A = new float[M * N];
    B = new float[N * P];
    C = new float[M * P];

    for (int m = 0; m < M; m++)
    {
        for (int n = 0; n < N; n++)
        {
            A[m * N + n] = GetAlpha(m) * cos(degToRad((PI * (2 * n + 1) * m / 16)));
            B[m * N + n] = GetAlpha(n) * cos(degToRad((PI * (2 * m + 1) * n / 16)));
        }
    }

    cout << endl << setw(10) << "A 출력" << endl;
    PrintMatrix(A, M, N);

    cout << endl << setw(10) << "B 출력" << endl;
    PrintMatrix(B, M, N);

    cout << endl << setw(10) << "AB 출력" << endl;
    matrixMultiplication(A, B, C, M, N, P);
    PrintMatrix(C, M, N);

    cout << endl << setw(10) << " A' - A의 2,5행 교환" << endl;
    float *A2 = new float[M * N];
    swapRow(M, N, A, A2, 2, 5);
    PrintMatrix(A2, M, N);

    cout << endl << setw(10) << " B' - B의 3,4열 교환" << endl;
    float *B2 = new float[M * N];
    swapCol(N, P, B, B2, 3, 4);
    PrintMatrix(B2, M, N);

    cout << endl << setw(10) << "A'B' 출력 " << endl;
    float *A2B2 = new float[M * N];
    matrixMultiplication(A2, B2, A2B2, M, N, P);
    PrintMatrix(A2B2, M, N);

    return 0;
}

float GetAlpha(int n)
{
    if (n == 0)
    {
        return 1 / (2 * sqrt(2));
    }
    else if (n >= 1 && n < 7)
    {
        return 0.5f;
    }
    return -1;
}

void swapRow(int m, int n, float *mA, float *mAd, int ii, int jj)
{
    memcpy(mAd, mA, m * n * sizeof(float));
    for (int i = 0; i < n; i++)
    {
        mAd[n * ii + i] = mA[n * jj + i];
        mAd[n * jj + i] = mA[n * ii + i];
    }
}

void swapCol(int m, int n, float *mA, float *mAd, int ii, int jj)
{
    memcpy(mAd, mA, m * n * sizeof(float));
    for (int i = 0; i < m; i++)
    {
        mAd[n * i + ii] = mA[n * i + jj];
        mAd[n * i + jj] = mA[n * i + ii];
    }
}

float degToRad(float deg)
{
    return deg * PI / 180.f;
}

int matrixMultiplication(float *a, float *b, float *c, int m, int n, int p)
{
    float dum;
    for (int j = 0; j < m; j++)
    {
        for (int k = 0; k < p; k++)
        {
            dum = 0;
            for (int l = 0; l < n; l++)
            {
                dum += a[j * n + l] * b[l * p + k];
            }
            c[j * p + k] = dum;
        }
    }

    return 1;
}

void PrintMatrix(float *a, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << setw(10) << a[i * n + j];
        }
        cout << endl;
    }
}
