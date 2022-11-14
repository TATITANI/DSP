#include <iostream>
#include <fstream>
#include <string>
#include "complex.h"
using namespace std;

const int H = 64, W = 64;
#define PI 3.141592

const int headersize = 54; // bmp 헤더 사이즈
char header[headersize];

// 흑백이므로 R성분만 리턴
unsigned char **ReadImage(ifstream &InFile, const char *fileName)
{
	InFile.open(fileName, ios::binary);

	// char *header = new char[headersize];
	InFile.read((char *)header, headersize);
	int H = 64, W = 64;

	unsigned char **R;	 // 영상의 저장 공간
	unsigned char **G;	 // 영상의 저장 공간
	unsigned char **B;	 // 영상의 저장 공간
	unsigned char **RGB; // 영상의 저장 공간

	R = new unsigned char *[H];
	G = new unsigned char *[H];
	B = new unsigned char *[H];
	RGB = new unsigned char *[H];
	for (int i = 0; i < H; i++)
	{
		R[i] = new unsigned char[W]; // R[64][64]
		G[i] = new unsigned char[W];
		B[i] = new unsigned char[W];
		RGB[i] = new unsigned char[3 * W]; /////
										   // 입력 영상이 흑백 영상이기에 r=g=b 값을 가진다.
										   // 입력 영상이 흑백 영상이 아니였다면 r, g, b에 대하여 각각 실행
	}

	for (int i = 0; i < H; i++)
		InFile.read((char *)RGB[i], 3 * W);

	for (int i = 0; i < H; i++)
	{
		for (int j = 0, jj = 0; j < W; j++, jj += 3)
		{
			B[i][j] = RGB[i][jj];
			G[i][j] = RGB[i][jj + 1]; //
			R[i][j] = RGB[i][jj + 2]; //
									  // bmp 파일은 R, G, B 순서가 아니라 B, G, R 순서로 저장되어 있습니다. (bmp만 예외적)
		}
	}

	for (int i = 0; i < H; i++)
	{
		delete[] G[i], B[i], RGB[i];
	}
	delete[] G, B, RGB;

	return R;
}

void WriteImage(ofstream &Result, const char *fileName, complex **R_)
{
	Result.open(fileName, ios::binary);
	Result.write((char *)header, headersize);

	unsigned char **RGB; // 영상의 저장 공간
	RGB = new unsigned char *[H];

	for (int i = 0; i < H; i++)
	{
		RGB[i] = new unsigned char[3 * W]; /////
		for (int j = 0, jj = 0; j < W; j++, jj += 3)
		{
			RGB[i][jj] = R_[i][j].re;
			RGB[i][jj + 1] = R_[i][j].re; //
			RGB[i][jj + 2] = R_[i][j].re; //
										  // bmp 파일은 R, G, B 순서가 아니라 B, G, R 순서로 저장되어 있습니다. (bmp만 예외적)
		}
	}

	for (int i = 0; i < H; i++)
	{
		Result.write((char *)RGB[i], 3 * W);
	}

	Result.close();

	for (int i = 0; i < H; i++)
	{
		delete[] RGB[i];
	}
	delete[] RGB;
}

complex **GenerateDFT(unsigned char **R, double &max)
{
	complex **dft;
	dft = new complex *[H];
	for (int i = 0; i < H; i++)
	{
		dft[i] = new complex[W];
	}

	for (int v = 0; v < H; v++)
	{
		for (int u = 0; u < W; u++)
		{
			for (int y = 0; y < H; y++)
			{
				for (int x = 0; x < W; x++)
				{
					dft[v][u] += complex(cos(-2 * PI * (u * x / (double)W + v * y / (double)H)),
										 sin(-2 * PI * (u * x / (double)W + v * y / (double)H))) *
								 (int)R[y][x];
				}
			}
			if (dft[v][u].mag() > max)
			{
				max = dft[v][u].mag();
			}
		}
	}

	return dft;

#pragma region DFT(2D)

#pragma endregion

#pragma region DFT check EXCEL

#pragma endregion
}

complex **NormalizeDFT(complex **dft, double &max)
{
	////////// 정규화 과정  /////////////////////////////////
	// 앞의 양자화 방법은 사람의 눈으로 확인하기에 어려울 수 있기 때문에
	// 아래의 방법을 통해서 사람 눈에 적합한 주파수 영상을 생성함

	// 0-255 양자화한 mag를 영상으로 저장
	complex **normalizedDFT;
	normalizedDFT = new complex *[H];
	// PPT Check DFT value 부분
	for (int v = 0; v < H; v++)
	{
		normalizedDFT[v] = new complex[W];
		for (int u = 0; u < W; u++)
		{
			normalizedDFT[v][u].re = (dft[v][u].mag() / max) * 255;
		}
	}

	return normalizedDFT;
}

complex ** GenerateIDFT(complex **dft)
{
#pragma region IDFT(2D)
	complex **R_; // DFT, IDFT 저장공간
	R_ = new complex *[H];
	for (int i = 0; i < H; i++)
	{
		R_[i] = new complex[W];
	}

	// 2D IDFT 선언
	for (int y = 0; y < H; y++)
	{
		for (int x = 0; x < W; x++)
		{
			for (int v = 0; v < H; v++)
			{
				for (int u = 0; u < W; u++)
				{
					R_[y][x] += complex(cos(-2 * PI * (u * x / (double)W + v * y / (double)H)),
										sin(-2 * PI * (u * x / (double)W + v * y / (double)H))) *
								dft[v][u];
				}
			}
			R_[y][x] = R_[y][x] / (double)(W * H);
		}
	}

#pragma endregion
return R_;
}

int main()
{
	ifstream InFile, InNoiseFile;
	unsigned char **R = ReadImage(InFile, "twin_64.bmp");
	// ReadImage(InFile, "twin_noise_64.bmp");

	//원본
	double max1 = 0;
	complex **dft = GenerateDFT(R, max1);
	ofstream originDFT;
	complex **normDFT = NormalizeDFT(dft, max1);
	WriteImage(originDFT, "re_twin_DFT.bmp", normDFT);
	complex** R_ = GenerateIDFT(dft);
	ofstream originIDFT;
	WriteImage(originIDFT, "re_twin.bmp", R_);
	
	for (int i = 0; i < H; i++)
	{
		delete[] R_[i];
	}
	delete[] R_;

	return 0;
}