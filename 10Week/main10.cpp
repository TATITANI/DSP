#include<iostream>
#include<fstream>
#include <math.h>
#include "fft.h"
using namespace std;

#define PI 3.141592
#define H 256
#define W 256
#define uchar unsigned char

// 중심이동
void ShiftCenter(double ** Real, double **Imag)
{
    int centerX = W / 2, centerY = H / 2;
    for (int cx = 0; cx < W; cx++)
    {
        for (int cy = 0; cy < centerY; cy++)
        {
            int oppositeX = cx < centerX ? cx + centerX : cx - centerX;
            int oppositeY = cy + centerY;

            double oppositeValue = Real[oppositeX][oppositeY];
            Real[oppositeX][oppositeY] = Real[cx][cy];
            Real[cx][cy] = oppositeValue;

            oppositeValue = Imag[oppositeX][oppositeY];
            Imag[oppositeX][oppositeY] = Imag[cx][cy];
            Imag[cx][cy] = oppositeValue;
        }
    }
}

void ImageProcess(uchar ** RGB, ofstream &outSpectrum, ofstream *outRestore = nullptr)
{
    uchar **R = new uchar *[H];
    double **R_  = new double *[H];
    double **imag_  = new double *[H];
    double **spectrum  = new double *[H];

    for (int i = 0; i < H; i++)
    {
        R[i] = new uchar[W];
        R_[i] = new double[W];
		imag_[i] = new double[W];
		spectrum[i] = new double[W];
        for (int j = 0, jj = 0; j < W; j++, jj += 3)
        {
            R[i][j] = RGB[i][jj];
        }
    }

    FFT_2D(R, R_, imag_, W, H);
    ShiftCenter(R_, imag_);
    Compute_Spectrum(R_,imag_, W,H,spectrum);
    DNormalize2D(spectrum, R, W, H);


	for (int i = 0; i < H; i++)
	{
		for (int j = 0, jj = 0; j < W; j++, jj += 3)
		{
			RGB[i][jj] = R[i][j];
			RGB[i][jj + 1] = R[i][j];
			RGB[i][jj + 2] = R[i][j];
		}
        outSpectrum.write((char *)RGB[i], 3 * W);
	}

    // 영상 복원
    if(outRestore != nullptr)
    {
        double **R2 = new double *[H];
        double **imag2_ = new double *[H];

        for (int i = 0; i < H; i++)
        {
            R2[i] = new double[W];
            imag2_[i] = new double[W];
        }
       
        //필터
        for(int y=0;y<H; y++)
        {
            for(int x=0;x<W; x++)
            {
                if(x < W/2 -20 || x>  W/2 + 20 ||  y <  H/2 - 20 || y> H/2 + 20 )
                {
                    if(R[y][x] > 110)
                    {
                          R_[y][x] = 0;
                          imag_[y][x] = 0;
                    }
                }
            }

        }
        ShiftCenter(R_, imag_);
        IFFT_2D(R_, imag_, R2, imag2_, W, H);
        DNormalize2D(R2, R, W, H);
        
        for (int i = 0; i < H; i++)
        {
            for (int j = 0, jj = 0; j < W; j++, jj += 3)
            {
                RGB[i][jj] =     R[i][j];
                RGB[i][jj + 1] = R[i][j];
                RGB[i][jj + 2] = R[i][j];
            }
            outRestore->write((char *)RGB[i], 3 * W);

            delete[] R2[i];
            delete[] imag2_[i];
        }
        delete[] R2;
        delete[] imag2_;
    }

    for (int i = 0; i < H; i++)
    {
        delete[] R[i];
        delete[] R_[i];
        delete[] imag_[i];
    }
    delete[] R;
    delete[] R_;
    delete[] imag_;
}


int main()
{
#pragma region 변수 설정
	uchar **RGB ;
	RGB = new uchar *[H];
	for (int i = 0; i < H; i++)
	{
		RGB[i] = new uchar[3 * W];
    }

#pragma endregion

#pragma region 초기화 영역
	ifstream inFileOrigin, inFileNoise;
	inFileOrigin.open("twins_.bmp", ios::binary);
	inFileNoise.open("twins_noise.bmp", ios::binary);
	char *header = new char[54];

	inFileOrigin.read((char *)header, 54);
	inFileNoise.read((char *)header, 54);
	
	ofstream outFileOriginSpectrum, outFileNoiseSpectrum, outFileRestore; // case 1,2,3 결과 이미지 파일
	outFileOriginSpectrum.open("twin_spectrum.bmp", ios::binary);
	outFileNoiseSpectrum.open("noise_spectrum.bmp", ios::binary);
	outFileRestore.open("noise_restore.bmp", ios::binary);
	outFileOriginSpectrum.write((char *)header, 54);
	outFileNoiseSpectrum.write((char *)header, 54);
	outFileRestore.write((char *)header, 54);

#pragma endregion

#pragma region RGB 분리

#pragma endregion

    // 원본 영상 스펙트럼 출력
    for (int i = 0; i < H; i++)
        inFileOrigin.read((char *)RGB[i], 3 * W);
    ImageProcess(RGB, outFileOriginSpectrum);

    //노이즈 영상 스펙트럼 출력, 복원 영상 출력
    for (int i = 0; i < H; i++)
        inFileNoise.read((char *)RGB[i], 3 * W);
    ImageProcess(RGB, outFileNoiseSpectrum, &outFileRestore);
    

#pragma region 메모리 해제 
delete header;
for (int i = 0; i < H; i++) {
	delete[] RGB[i];

}
delete[] RGB;

#pragma endregion

#pragma region 파일 닫기 
inFileOrigin.close();
inFileNoise.close();

outFileOriginSpectrum.close();
outFileNoiseSpectrum.close();
outFileRestore.close();
#pragma endregion

system("pause");


return 0;
}
