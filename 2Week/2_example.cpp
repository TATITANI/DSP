#include <iostream>
using namespace std;

int matrixMultiplication(int *a, int *b, int *c, int m, int n, int p);

int main()
{
    int *A, *B, *C, m, n, p;
    m = 2;
    n = 3, p = 2;
    A = new int[m * n];
    B = new int[n * p];
    C = new int[m * p];
    A[0] = 2;
    A[1] = 3;
    A[2] = 4;
    *(A + n) = 5;
    A[n * 1 + 1] = 6;
    A[5] = 7;
    
    B[0] = 2;
    B[1] = 3;
    B[2] = 5;
    B[3] = 7;
    B[4] = 1;
    *(B + p * 2 + 1) = 3;
    matrixMultiplication(A, B, C, m, n, p);
    cout << "A" << endl;
    for (int mm = 0; mm < m; mm++)
    {
        cout << "\n";
        for (int nn = 0; nn < n; nn++)
            cout << "\t" << A[mm * n + nn];
    }
    return 0;
}

int matrixMultiplication(int *a, int *b, int *c, int m, int n, int p)
{
    int dum;
    for (int j = 0; j < m; j++)
    {
        for (int k = 0; k < p; k++)
        {
            dum = 0;
            for (int l = 0; l < n; l++)
                dum += a[j * n + l] * b[l * p + k];
            c[j * p + k] = dum;
        }
    }
    return 1;
}
