#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>
#include <algorithm> // sort
#include <iostream>
using namespace std;

#define WORD unsigned short
#define DWORD unsigned int
#define NN 4096
#define PI 3.141592
int N = 1024;

template <typename T>
vector<T> Convolution(vector<T> x, vector<T> h)
{
    const int len = x.size() + h.size() - 1;

    vector<T> result(len);
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

int main() {
    // 1. Read the wav file.
    char* f = new char[44];
    FILE* fff, * fout;
     fopen_s(&fff, "noiseMusic.wav", "rb");
    if (!fff) {
        printf("cant open noiseMusic.wav.\n");
        exit(123);
    }
    // 2. Read the first 44 bytes
    if ((fread(f, 1, 44, fff) != 44)) {
        printf("cant read noiseMusic.wav..\n");
        exit(444);
    }
    fopen_s(&fout, "out_Music1.wav", "wb");
    if (!fout) {
        printf("cant open out_Music1.wav.\n");
        exit(123);
    }

    int size_music = (int)(*(DWORD*)(f + 4)) + 8 - 44;
    //printf("size_music : %d\n", size_music);

    if ((fwrite(f, 1, 44, fout) != 44)) {
        printf("cant write header on out_Music1.wav. \n");
    }

    size_music /= 4;  // 4bytes per sample (스테레오)

    // h[n] 정의
    vector<float> h;
    const int L = 25;
    for (int n = 0; n < L; n++)
    {
        float a=  sin(PI / 4 * ((float)n - (float)(L - 1) / 2.f)) ;
        float b= (PI * ((float)n - (float)(L - 1) / 2.f));
        float value = b!=0 ? a/ b : INT_MAX;
        cout << n << ", " <<  value << endl;      
        h.push_back(value);
    }

    const int N = size_music;
    short* leftdata = new short[N];
    short* rightdata = new short[N];
    short* alldata = new short[N*2];
    for (int n = 0; n < size_music; n += N) {  // all data
        if ((fread(alldata, 4, N, fff)) != N) {
        }
        // store left/right data
        int ii, iii;
        for (ii = iii = 0; ii < N; ii++, iii += 2) {
            leftdata[ii] = alldata[iii];
            rightdata[ii] = alldata[iii + 1];
        }

        // x[n] 정의 (기본 음원)
        vector<float> xL;
        vector<float> xR;
        for (int n = 0; n <  N; n++)
        {
            xL.push_back(leftdata[n]);
            xR.push_back(rightdata[n]);
        }

        vector<float> resultL = Convolution(xL, h);
        vector<float> resultR = Convolution(xR, h);
 
        // restore left/right data
        for (ii = iii = 0; ii < N; ii++, iii += 2) {
            alldata[iii] = resultL[ii];
            alldata[iii + 1] = resultR[ii];
        }

        // 5. write on output file
        if (fwrite(alldata, 4, N, fout) != N) {
            printf("cant write %d-th .wav. \n", n);
        }
    }  // all data

    fclose(fff);
    fclose(fout);
    delete f;
    delete leftdata;
    delete rightdata;
    delete alldata;

    return 1;
}
