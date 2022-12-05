#include <iostream>
#include <Vector>
#include <cmath>
#include <fstream>
#include "complex.h"
using namespace std;
#define WORD unsigned short
#define DWORD unsigned int
#define PI 3.141592
// #define N 64

#define MusicSize 32000 // 16000

void Z_Function(double x0, double x1, double x2, double y0, double y1, double y2, complex* H, ofstream &out_k, ofstream &out_mag)
{
    complex zeros1, zeros2, poles1, poles2;
    double Zeros, Poles;

    Zeros = x1 * x1 - 4 * x0 * x2; // 판별식

    if (Zeros > 0)
    { // 서로 다른 두 실근
        zeros1 = complex((-x1 - sqrt(x1 * x1 - 4 * x0 * x2)) / (2 * x0), 0);
        zeros2 = complex((-x1 + sqrt(x1 * x1 - 4 * x0 * x2)) / (2 * x0), 0);
    }
    else if (Zeros == 0)
    { // 중근
        zeros1 = zeros2 = complex((-x1) / (2 * x0), 0);
    }
    else
    { // 허근
        zeros1 = complex((-x1) / (2 * x0), sqrt(abs(x1 * x1 - 4 * x0 * x2)) / (2 * x0));
        zeros2 = complex((-x1) / (2 * x0), -sqrt(abs(x1 * x1 - 4 * x0 * x2)) / (2 * x0));
    }

    Poles = y1 * y1 - 4 * y0 * y2; // 판별식

    if (Poles > 0)
    {
        poles1 = complex((-y1 - sqrt(y1 * y1 - 4 * y0 * y2)) / (2 * y0), 0);
        poles2 = complex((-y1 + sqrt(y1 * y1 - 4 * y0 * y2)) / (2 * y0), 0);
    }
    else if (Poles == 0)
    {
        poles1 = poles2 = complex((-y1) / (2 * y0), 0);
    }
    else
    {
        poles1 = complex((-y1) / (2 * y0), sqrt(abs(y1 * y1 - 4 * y0 * y2)) / (2 * y0));
        poles2 = complex((-y1) / (2 * y0), -sqrt(abs(y1 * y1 - 4 * y0 * y2)) / (2 * y0));
    }

    const int len = _msize(H)/sizeof(complex);
    complex *Z = new complex[len];

    for (int k = 0; k < len; k++)
    {
        Z[k] = complex(2 * PI * k / (double)len);
        H[k] = (Z[k] - zeros1) * (Z[k] - zeros2) / ((Z[k] - poles1) * (Z[k] - poles2));
        out_k << k << endl;
        out_mag << H[k].mag() << endl;
    }
}

void Dirichlet(complex *H, int L, int fs, ofstream &out_k, ofstream &out_mag)
{
    complex upper, bottom;
    double lim = L; // 11.;

    for (int k = 0; k < fs; k++)
    {
        bottom = complex(sin(2. * PI * k / (double)(2. * fs)), 0.);
        if (bottom.mag() == 0.0)
        {
            H[k] = complex(cos(-2. * PI * k * ((L - 1) / 2.) / (double)fs), sin(-2. * PI * k * ((L - 1) / 2.) / (double)fs)) * lim;
        }
        else
        {
            upper = complex(sin(2. * PI * k * L / (double)(2. * fs)), 0.0);
            H[k] = upper / bottom * complex(cos((double)(-2. * PI * k * ((L - 1) / 2.)) / (double)fs), sin(-2. * PI * k * ((L - 1) / 2.) / (double)fs));
        }
    }

    for (int k = 0; k < fs; k++)
    {
        out_k << k << endl;
        out_mag << H[k].mag() << endl;
    }
}

int header_size = 44;                 // wav 헤더 44 바이트
char *header = new char[header_size]; // 헤더 저장 공간

void ReadHeader(ifstream &fff)
{
    fff.read((char *)header, header_size); // 헤더 정보 읽기

#pragma region HEADERSHOW // 헤더 정보 출력하기
    // cout << "RIFF     " << header[0] << header[1] << header[2] << header[3] << endl;
    // cout << "filesize - 8 " << *(DWORD *)(header + 4) << endl;
    // cout << "WAVE     " << header[8] << header[9] << header[10] << header[11] << endl;
    // cout << "cksize   " << *(DWORD *)(header + 16) << endl;
    // cout << "channels " << *(WORD *)(header + 22) << endl;
    // cout << "fs       " << *(DWORD *)(header + 24) << endl;
    // cout << "bytes/s  " << *(DWORD *)(header + 28) << endl;
    // cout << "bits/sam " << *(WORD *)(header + 34) << endl;
    // cout << "data     " << header[36] << header[37] << header[38] << header[39] << endl;
    // cout << "cksize   " << *(DWORD *)(header + 40) << endl; // sub-chunk 2 size
#pragma endregion
}

void WriteDFT(ifstream &_ifstream, char *fileName, complex *X)
{
    short data[MusicSize];
    char *header = new char[header_size];
    _ifstream.read((char *)header, header_size);
    _ifstream.read((char *)data, MusicSize);

    // DFT
    for (int k = 0; k < MusicSize; k++)
    {
        for (int n = 0; n < MusicSize; n++)
        {
            X[k] += complex(cos((-2 * PI * k * n) / (double)MusicSize),
                            sin((-2 * PI * k * n) / (double)MusicSize)) *
                    data[n];
        }
    }
    ofstream out;
    out.open(fileName);
    for (int k = 0; k < MusicSize; k++)
    {
        // int id = k - MusicSize / 2;
        out << k << "," << X[k].mag() << endl;
    }
    out.close();
}

void IDFT(complex *X, short *result)
{
    complex *x = new complex[MusicSize];
    for (int n = 0; n < MusicSize; n++)
    {
        for (int k = 0; k < MusicSize; k++)
        {
            x[n] += X[k] * complex(cos((2 * PI * k * n) / (double)MusicSize),
                                   sin((2 * PI * k * n) / (double)MusicSize));
        }
        x[n] = x[n] / (double)MusicSize;
        result[n] = x[n].re;
    }
}

void Problem1()
{
    //디리클레 필터
    ofstream out_k, out_mag;
    int fs = 3000; 
    int L = 11;

    out_k.open("1.txt");
    out_mag.open("mag1.txt");
    if ((!out_k.is_open()) && (!out_mag.is_open()))
    {
        cerr << "error for new file" << endl;
        exit(101);
    }
    complex *H = new complex[fs];
    Dirichlet(H, L, fs, out_k, out_mag);

    // 음악파일
    ifstream originFile;
    originFile.open("noiseMusic.wav", ios::binary); // 불러올 음악 파일 선택 (바이너리로 읽기)
    if (!originFile)                                // 음악 파일이 존재하는지 확인
    {
        cerr << "can't open musicA.wav" << endl;
        exit(123);
    }
    ReadHeader(originFile);

    complex *X = new complex[MusicSize];
    WriteDFT(originFile, "originFreq1.txt", X);

    ofstream resultFile;
    resultFile.open("resultFreq1.txt");
    for (int k = 0; k < MusicSize; k++)
    {
        // int freq = k- MusicSize/2;
        if (k < 3000)
        {
            X[k] = H[k] * X[k];
        }else
        {
            X[k] = 0;
        }
        resultFile << k << " , " << X[k].mag() << endl;
    }

    short result[MusicSize];
    IDFT(X,result);
    
    // wav파일 출력
    ofstream resultWav;
    resultWav.open("resultWav1.wav", ios::binary);
    resultWav.write(header, header_size);
    resultWav.write((char *)result, MusicSize);
    
    resultFile.close();
    resultWav.close();
    originFile.close();

    delete X;
}


void Problem2()
{
    double a1, b1, c1, a2, b2, c2;
    
    ofstream out_k, out_mag;
    out_k.open("k2.txt");
    out_mag.open("mag2.txt");

    a1 = 1;
    b1 = 0;
    c1 = -0.64;

    a2=1;
    b2 = -0.4;
    c2 = 0.68;

    complex *H = new complex[10000];

    Z_Function(a1,b1,c1, a2,b2,c2, H, out_k, out_mag);
    
    // 음악 파일
    ifstream originFile;
    originFile.open("noiseMusic.wav", ios::binary); // 불러올 음악 파일 선택 (바이너리로 읽기)
    if (!originFile)                                // 음악 파일이 존재하는지 확인
    {
        cerr << "can't open musicA.wav" << endl;
        exit(123);
    }
    ReadHeader(originFile);

    complex *X = new complex[MusicSize];
    WriteDFT(originFile, "originFreq2.txt", X);

    ofstream resultFile;
    resultFile.open("resultFreq2.txt");
    for (int k = 0; k < MusicSize; k++)
    {
        if (k >= 15000 && k<25000)
        {
            X[k] = H[k-15000] * X[k];
        }else
        {
            X[k] = 0;
        }
        resultFile << k << " , " << X[k].mag() << endl;
    }
    // wav파일 출력
    short result[MusicSize];
    IDFT(X, result);
    
    ofstream resultWav;
    resultWav.open("resultWav2.wav", ios::binary);
    resultWav.write(header, header_size);
    resultWav.write((char *)result, MusicSize);
    
    resultFile.close();
    resultWav.close();
    originFile.close();

    delete X;

}

int main()
{
    Problem1();
    Problem2();

    return 1;
}

