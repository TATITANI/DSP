#include <iostream>
#include <Vector>
#include <cmath>
#include <fstream>
#include "complex.h"
using namespace std;
#define WORD unsigned short
#define DWORD unsigned int
#define PI 3.141592

#define MusicSize 32000 // 16000
/* system example : DIR */

int header_size = 44;                 // wav 헤더 44 바이트
char *header = new char[header_size]; // 헤더 저장 공간

void ReadHeader(ifstream &fff)
{
    fff.read((char *)header, header_size); // 헤더 정보 읽기

#pragma region HEADERSHOW // 헤더 정보 출력하기
    cout << "RIFF     " << header[0] << header[1] << header[2] << header[3] << endl;
    cout << "filesize - 8 " << *(DWORD *)(header + 4) << endl;
    cout << "WAVE     " << header[8] << header[9] << header[10] << header[11] << endl;
    cout << "cksize   " << *(DWORD *)(header + 16) << endl;
    cout << "channels " << *(WORD *)(header + 22) << endl;
    cout << "fs       " << *(DWORD *)(header + 24) << endl;
    cout << "bytes/s  " << *(DWORD *)(header + 28) << endl;
    cout << "bits/sam " << *(WORD *)(header + 34) << endl;
    cout << "data     " << header[36] << header[37] << header[38] << header[39] << endl;
    cout << "cksize   " << *(DWORD *)(header + 40) << endl; // sub-chunk 2 size
#pragma endregion
}

void WriteDFT(ifstream &_ifstream, char *fileName, complex *X)
{
    short data[MusicSize];
    char * header = new char[header_size];
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
        int id = k - MusicSize / 2;
        out << id << "," << X[k].mag() << endl;
    }
    out.close();
}

void ModifyNoise(ifstream &_ifstream, char *wavName, complex *X)
{
    // IDFT
    short result[MusicSize];
    complex *x_ = new complex[MusicSize];
    for (int n = 0; n < MusicSize; n++)
    {
        for (int k = 0; k < MusicSize; k++)
        {
            if (X[k].mag() > 140000000)
            {
                X[k].re = 0;
                X[k].im = 0;
            }
            x_[n] += X[k] * complex(cos((2 * PI * k * n) / (double)MusicSize),
                                    sin((2 * PI * k * n) / (double)MusicSize));
        }
        x_[n] = x_[n] / (double)MusicSize;
        result[n] = x_[n].re;
    }

    ofstream out;
    out.open(wavName, ios::binary);
    out.write(header, header_size);
    out.write((char *)result, MusicSize);
    out.close();
}

int main()
{
    ifstream originFile, mixFile;
    originFile.open("musicA.wav", ios::binary); // 불러올 음악 파일 선택 (바이너리로 읽기)
    mixFile.open("MixA.wav", ios::binary);      // 불러올 음악 파일 선택 (바이너리로 읽기)
    if (!originFile)                            // 음악 파일이 존재하는지 확인
    {
        cerr << "can't open musicA.wav" << endl;
        exit(123);
    }
    cout << "=== original wav header info===" << endl;
    ReadHeader(originFile);

#pragma region Homework // 과제 코드 작성
    complex *X_origin = new complex[MusicSize];
    complex *X_mix = new complex[MusicSize];
    WriteDFT(originFile, "out_origin.txt", X_origin);
    WriteDFT(mixFile, "out_mix.txt", X_mix);
    ModifyNoise(mixFile, "removeNoise.wav", X_mix);

#pragma endregion

    originFile.close();
    mixFile.close();

    delete[] header;
    delete X_origin;
    delete X_mix;
    return 1;
}