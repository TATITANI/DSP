#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
using namespace std;

#define PI 3.141592

template <typename T>
vector<T> Convolution(vector<T> x, vector<T> h)
{
    const int N = x.size() + h.size() - 1;

    vector<T> result(N);
    for (int n = 0; n < N; n++)
    {
        for (int k = 0; k < x.size(); k++)
        {
            if (n - k >= h.size() || (n - k) < 0)
            {
                continue;
            }
            result[n] += x[k] * h[n - k];
        }
    }

    return result;
}

void Problem1()
{
    vector<int> x = {1, 1, 1, 1, 1};
    vector<int> h1 = {1, 0, 0, 0, 0, 0, 6};
    vector<int> h2 = {1, 2, 3, 2, 1};

    vector<int> result = Convolution(x, h1);
    result = Convolution(result, h2);
    for(int id = 0; id< result.size(); id++)
    {
        cout << id<< ", " << result[id]<< endl;

    }
    cout << endl;
}

void Problem2()
{
    vector<float> cosArr;
    vector<float> expArr;
    for (float t = -1; t <= 1; t += 0.1f)
    {
        cosArr.push_back(cos(2 * PI * t));
        expArr.push_back(t <0 ? 0 : exp(-t));
    }
    vector<float> result = Convolution(cosArr, expArr);
     for(int id = 0; id< result.size(); id++)
    {
        cout << id<< ", " << result[id]<< endl;

    }
    cout << endl;
}

int main()
{
    cout << "=== problem 1 ===\n";
    Problem1();
    cout << "=== problem 2 ===\n";
    Problem2();
}
