#include <iostream> // cout, cin
#include <fstream>  // ifstream, ofstream 입출력 파일 라이브러리
#include <math.h>

using namespace std;
#define PHI 3.141592

int main()
{

    ofstream outFile;                         // 출력 파일 선언.
    outFile.open("data2_3600.txt", ios::out); // 출력 파일 data.txt 열기.

    int fc = 1600;                        // carrier frequency
    float t = 0, fs = 3600, dt = 1. / fs; // 시간, 샘플링 주파수, 샘플링 주기
    int f0 = 200, n = 20, smp_cnt;        // 기본주파수, n개 주기 파형, 신호 샘플 개수
    smp_cnt = (fs / f0) * n;              // 샘플링 횟수 = 한 주기 동안 샘플링하는 횟수 * 샘플링할 주기의 수

    // n = nTs이므로 시간 t에 루프마다 샘플링 주기를 더해준다.
    for (int i = 0; i <= smp_cnt; i++, t += dt)
    {
        //샘플링 데이터를 텍스트에 기록한다.
        outFile << t << "\t " << cos(2 * PHI * f0 * t) * sin(2 * PHI * fc * t) << endl;
    }
    outFile.close();

    return 0;
}
