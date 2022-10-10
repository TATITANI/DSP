#include <iostream> // cout, cin
#include <fstream>  // ifstream, ofstream 파일 라이브러리
#include <math.h>   // sin, cos
using namespace std;
#define Width 300
#define Height 200
#define PI 3.141592

class Display
{
private:
    unsigned char *RGB;
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
    const char *getRGB() { return (const char *)RGB; }
};

void DrawCircle(Display &display, int radius, int thickness);

int main()
{
    ofstream fff("video.rgb", ios::out | ios::binary); // 출력 파일 선언.바이너리
    Display display(Width, Height);                    // 이미지 저장 공간

    // 동영상 만들 때
    for (int frame = 0; frame < 10; frame++)
    {
        for (int x = 0; x < Width; x++)
        {
            for (int y = 0; y < Height; y++)
            {
                display.SetColor(x, y, 0, 255, 0); //배경 파랑
            }
        }
        // 원 그리기
        DrawCircle(display, 10 * frame, 6);
        fff.write(display.getRGB(), 3 * Width * Height); // 동영상의 한 프레임 저장
    }
    fff.close();
    return 0;
}

void DrawCircle(Display &display, int radius, int thickness)
{
    // 원의 중심점 좌표
    const int centerX = Width / 2;
    const int centerY = Height / 2;
    // 연산 비용을 줄이기 위해 0~90도까지만 반복문 실행.
    for (float rad = 0; rad <= PI * 0.5f; rad += PI * 0.002f)
    {
        //두께
        for (int t = -thickness/2; t < thickness/2; t++)
        {
            //원점 기준 원의 점 좌표
            int x = (radius + t) * cos(rad);
            int y = (radius + t) * sin(rad);
            // 대칭하는 좌표 4곳에 분홍색을 입힌다.
            display.SetColor(centerX + x, centerY + y, 255, 0, 255);
            display.SetColor(centerX + x, centerY - y, 255, 0, 255);
            display.SetColor(centerX - x, centerY + y, 255, 0, 255);
            display.SetColor(centerX - x, centerY - y, 255, 0, 255);
        }
    }
}
