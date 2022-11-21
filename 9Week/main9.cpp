#include<iostream>
#include<fstream>
#include <math.h>
using namespace std;

#define PI 3.141592
#define uchar unsigned char
#define H 256
#define W 256

void DCT(uchar** img, double** dct, int filter[8][8]);
void IDCT(double** dct, uchar** result_img);


void HW1(double** dct, uchar** HW1_img,char* header);

void DCT_1D(double* data, double* dct);
void IDCT_1D(double* dct, double* result_data);

void GenerateCase(uchar **R, double **dct, int filter[8][8], uchar **R_, ofstream &outFile)
{
	DCT(R, dct, filter);
	IDCT(dct, R_);

#pragma region R, G, B 결합
	uchar RGB[H][3*W];
	for (int i = 0; i < H; i++)
	{
		for (int j = 0, jj = 0; j < W; j++, jj += 3)
		{
			RGB[i][jj] = R_[i][j];
			RGB[i][jj + 1] = R_[i][j];
			RGB[i][jj + 2] = R_[i][j];
		}
	}

	for (int i = 0; i < H; i++)
	{
		outFile.write((char *)RGB[i], 3 * W);
	}
}

int main()
{
#pragma region 변수 설정 
	uchar** RGB;
	uchar** R;
	uchar** G;
	uchar** B;
	
	uchar** R_;
	uchar** G_;
	uchar** B_;
	uchar** RGB_;

	double** dct;

	RGB = new uchar*[H];
R = new uchar*[H];
G = new uchar*[H];
B = new uchar*[H];
R_ = new uchar*[H];
G_ = new uchar*[H];
B_ = new uchar*[H];
RGB_ = new uchar*[H];
dct = new double*[H];
for (int i = 0; i < H; i++) {
	RGB[i] = new uchar[3 * W];
	R[i] = new uchar[W];
	G[i] = new uchar[W];
	B[i] = new uchar[W];
	R_[i] = new uchar[W];
	G_[i] = new uchar[W];
	B_[i] = new uchar[W];
	RGB_[i] = new uchar[3 * W];
	dct[i] = new double[W];
}

#pragma endregion

#pragma region 초기화 영역 
ifstream Infile;


Infile.open("twin_.bmp", ios::binary);
char* header = new char[54];

Infile.read((char*)header, 54);
for (int i = 0; i < H; i++)
	Infile.read((char*)RGB[i], 3 * W);

ofstream outFile1, outFile2, outFile3; // case 1,2,3 결과 이미지 파일
outFile1.open("twin_out1.bmp", ios::binary);
outFile2.open("twin_out2.bmp", ios::binary);
outFile3.open("twin_out3.bmp", ios::binary);
outFile1.write((char *)header, 54);
outFile2.write((char *)header, 54);
outFile3.write((char *)header, 54);

#pragma endregion 

#pragma region RGB 분리
for (int i = 0; i < H; i++) {
	for (int j = 0, jj = 0; j < W; j++, jj += 3) {
		B[i][j] = RGB[i][jj];
		G[i][j] = RGB[i][jj + 1];
		R[i][j] = RGB[i][jj + 2];
	}
}
#pragma endregion

int filter1[8][8] = 
{
	1,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,0,
	1,1,1,1,1,0,0,0,
};
int filter2[8][8] = 
{
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,
};
int filter3[8][8] = 
{
	0,0,0,1,1,1,1,1,
	0,0,0,1,1,1,1,1,
	0,0,0,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
};

GenerateCase(R, dct, filter1, R_, outFile1);// case 1
GenerateCase(R, dct, filter2, R_, outFile2);// case 2
GenerateCase(R, dct, filter3, R_, outFile3);// case 3


// 압선 DFT보다 매우 빠른 연산 속도 (DFT는 1024x1024 영상을 처리하는데 12일이 필요함) 
// 고주파수에 해당하는 영역을 제거하여도
// Input 영상과 Output 영상이 크게 달라지지 않음을 확인 
// - DCT가 압축 분야에서 활용되는 이유 중 하나 (멀티미디어 수업) 

#pragma region 메모리 해제 
for (int i = 0; i < H; i++) {
	delete[] RGB[i];
	delete[] R[i];
	delete[] G[i];
	delete[] B[i];
	delete[] RGB_[i];
	delete[] dct[i];
}
delete[] RGB;
delete[] R;
delete[] G;
delete[] B;
delete[] RGB_;
delete[] dct;

#pragma endregion

#pragma region 파일 닫기 
Infile.close();

outFile1.close();
outFile2.close();
outFile3.close();
#pragma endregion

system("pause");
return 0;
}

// 2D DCT 작성
void DCT(uchar** img, double** dct, int filter[8][8])
{
	const int macroH = 8, macroW = 8;
	for (int v = 0; v < H; v++)
    {
        for (int u = 0; u < W; u++)
        {
            int v1 = v % macroH; // macroblock 에서의 uv좌표
            int u1 = u % macroW;
            double sum = 0;
            for (int j = 0; j < macroH; j++)
            {
                int fy = j + macroH * (v / macroH);
                for (int i = 0; i < macroW; i++)
                {
                    int fx = i + macroW * (u / macroW);
                    sum += cos((2 * i + 1) * u1 * PI / (double)(macroW * 2)) *
                              cos((2 * j + 1) * v1 * PI / (double)(macroH * 2)) *
                              img[fy][fx];
                }
            }

            double Cu = (u1 == 0) ? 1 / sqrt(2) : 1;
            double Cv = (v1 == 0) ? 1 / sqrt(2) : 1;
            
            dct[v][u] = sum * Cu * Cv / 4.f ;
			// 필터 적용
			dct[v][u] *= filter[v1][u1];
        }
    }
}
#define DATALENGTH 256

void DCT_1D(double* data, double* dct)
{
	int N = 8;
	int mcrNb = DATALENGTH / N;

	double sum = 0;

	int u, n;
	for (int mcr = 0; mcr < mcrNb; mcr++)
	{
		for (int k = 0; k < N; k++) {
			u = mcr * N + k;
			sum = 0;
			for (int i = 0; i < N; i++) {
				n = mcr * N + i;
				double theta = (double)(2. * i + 1)*k*PI / (2. * N);
				sum += (double)cos(theta)*data[n];
			}
			double ck = (k) ? 0.5 : sqrt((double)1.0 / (double)N);
			dct[k] = ck * sum;
		}

	}
}

void IDCT(double** dct, uchar** result_img)
{
	const int macroH = 8, macroW = 8;
	// 2D IDCT 작성
	 for (int j = 0; j < H; j++)
    {
        for (int i = 0; i < W; i++)
        {
            int j1 = j % macroH;
            int i1 = i % macroW;
            float sum = 0;
            for(int v=0; v< macroH; v++)
            {
                int Fv = v + macroH * (j / macroH);
                for (int u = 0; u < macroW; u++)
                {
                    int Fu = u + macroW * (i / macroW);

                    float Cu = (u == 0) ? 1 / sqrt(2) : 1;
                    float Cv = (v == 0) ? 1 / sqrt(2) : 1;
                    float x= cos((2 * i1 + 1) * u * PI / (float)(macroW * 2)) *
                           cos((2 * j1 + 1) * v * PI / (float)(macroH * 2)) *
                           dct[Fv][Fu] * Cu * Cv/4.f;
                    sum += x;
                }
            }
            result_img[j][i] = sum;
         }
    }

}

void IDCT_1D(double* dct, double* result_data)
{
	int N = 8;
	int mcrNb = DATALENGTH / N;

	double sum = 0;

	int u, n;

	for (int mcr = 0; mcr < mcrNb; mcr++)
	{
		for (int i = 0; i < N; i++) {
			n = mcr * N + i;
			sum = 0;
			for (int k = 0; k < N; k++) {
				u = mcr * N + k;
				double theta = (double)(2. * i + 1)*k*PI / (2. * N);
				double ck = (k) ? 0.5 : sqrt((double)1.0 / (double)N); // C(u) ->> C(k)

				sum += ck * cos(theta)*dct[u];
			}
			result_data[i] = sum;
		}
	}

}


