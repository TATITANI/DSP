#include <iostream> // cout, cin
#include <fstream>  // ifstream, ofstream 파일 라이브러리
#include <math.h>   // sin, cos

#include <stdio.h>
#include <stdlib.h>

using namespace std;
#define Width 600
#define Height 400
#define PI 3.141592

class Display
{
private:
    unsigned char* RGB;
    int w, h;

public:
    // 화면 크기에 따라 필요한 색상정보 동적 할당
    Display(int _w, int _h) : w(_w), h(_h)
    {
        RGB = new unsigned char[w * h * 3];
    }
    ~Display()
    {
        delete[] RGB;
    }
    void SetColor(int x, int y, int r, int g, int b)
    {
        int startID = (y * w + x) * 3;
        RGB[startID] = r;
        RGB[startID + 1] = g;
        RGB[startID + 2] = b;
    }
    const char* getRGB() { return (const char*)RGB; }
};

int main()
{
    int fileSize = 3 * Width * Height;
    unsigned char* inputImg = new  unsigned char [fileSize];
    FILE* inputFile;
    fopen_s(&inputFile, "image.rgb", "rb");
    if ( inputFile == NULL) {
        printf("cant open .\n");
        exit(123);
    }
    // 2. Read the first 44 bytes
    if ((fread(inputImg, 1, fileSize, inputFile) != fileSize)) {
        printf("cant read ..\n");
        exit(444);
    }
 
    ofstream fff("out_image.rgb", ios::out | ios::binary); // 출력 파일 선언.바이너리

    Display display(Width, Height); // 이미지 저장 공간
    unsigned char RGB[Width * Height * 3];


    int mask[3][3] = { {1,1,1},{1,-8,1},{1,1,1} };
    for (int x = 0; x < Width; x++)
    {
        for (int y = 0; y < Height; y++)
        {
            int r=0, g=0, b= 0;
            if (x > 0 && x < Width - 1 && y > 0 && y < Height - 1)
            {
                //convolution
                for (int dx = -1; dx <= 1; dx++)
                {
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        r += (int)inputImg[((y + dy) * Width + (x + dx)) * 3 + 2] * mask[dx + 1][dy + 1];
                        g += (int)inputImg[((y + dy) * Width + (x + dx)) * 3 + 1] * mask[dx + 1][dy + 1];;
                        b += (int)inputImg[((y + dy) * Width + (x + dx)) * 3 + 0] * mask[dx + 1][dy + 1];;
                    }
                }
            }
          
			display.SetColor(x, y, r, g, b);
        }
    }

    fff.write(display.getRGB(), 3 * Width * Height);
    fff.close();
    return 0;
}
