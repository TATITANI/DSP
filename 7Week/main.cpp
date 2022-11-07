#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>
#include <algorithm> // sort
#include <iostream>
#include "complex.h"
using namespace std;
#define WORD unsigned short
#define DWORD unsigned int
#define PI 3.141592

#define MusicSize 16000
/* system example : DIR */

void ReadHeader(FILE *_file)
{
	int header_size = 44;				  // wav 헤더 44 바이트
	char *header = new char[header_size]; // 헤더 저장 공간

	if (!_file) // 음악 파일이 존재하는지 확인
	{
		cerr << "can't open wav" << endl;
		exit(123);
	}

	if ((fread(header, 1, header_size, _file) != header_size))
	{
		printf("cant read wav..\n");
		exit(444);
	}

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

	delete[] header;
}

// void Write_DFT(File *_file, string outFileName)
// {
// 	short *data = new short[MusicSize];
// 	_file.read((char *)data, sizeof(data) * MusicSize);
// 	if ((fread(data, 1, MusicSize, _file) != MusicSize))
// 	{
// 		printf("cant read wav..\n");
// 		exit(444);
// 	}

// 	File *outFile1;
// 	fopen_s(&outFile1, outFileName , "wb");
// 	if (!outFile1)
// 	{
// 		printf("cant open wav.\n");
// 		exit(123);
// 	}

// 	complex *X1 = new complex[MusicSize];
// 	for (int k = 0; k < MusicSize; k++)
// 	{
// 		for (int n = 0; n < MusicSize; n++)
// 		{
// 			X1[k] += complex(cos((-2 * PI * k * n) / (double)MusicSize),
// 							 sin((-2 * PI * k * n) / (double)MusicSize)) *
// 					 data[n];
// 		}
// 		if (X1[k].re > 1)
// 			outFile1 << k << "," << (X1[k].mag()) << endl;
// 	}
// 	delete[] data;
// }

int main()
{
	FILE *originFile, *mixFile;
	fopen_s(&originFile, "musicA.wav", "rb");
	fopen_s(&mixFile, "mixA.wav", "rb");

	cout << "===원본 음원 헤더 정보===\n";
	ReadHeader(originFile);
	cout << "===노이즈 음원 헤더 정보===\n";
	ReadHeader(mixFile);

#pragma region Homework // 과제 코드 작성

	// Write_DFT(originFile, "out_origin.txt");
	// Write_DFT(mixFile, "out_mix.txt");

	// for (int i = 0; i < MusicSize; i++)
	// {
	// 	outFile1 << data[i] << endl;
	// 	// outFile.write((const char*)data[i], sizeof(data[i]));
	// }

	short *data = new short[MusicSize];
	originFile.read((char *)data, sizeof(data) * MusicSize);
	if ((fread(data, 1, MusicSize, originFile) != MusicSize))
	{
		printf("cant read wav..\n");
		exit(444);
	}

	File *outFile1;
	// fopen_s(&outFile1, "out_origin.wav", "wb");
	// if (!outFile1)
	// {
	// 	printf("cant open wav.\n");
	// 	exit(123);
	// }

	// complex *X1 = new complex[MusicSize];
	// for (int k = 0; k < MusicSize; k++)
	// {
	// 	for (int n = 0; n < MusicSize; n++)
	// 	{
	// 		X1[k] += complex(cos((-2 * PI * k * n) / (double)MusicSize),
	// 						 sin((-2 * PI * k * n) / (double)MusicSize)) *
	// 				 data[n];
	// 	}
	// 	if (X1[k].re > 1)
	// 		outFile1 << k << "," << (X1[k].mag()) << endl;
	// }
	delete[] data;

#pragma endregion

	return 1;
}