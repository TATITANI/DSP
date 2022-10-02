#include <iostream> // cout, cin
#include <fstream>  // ifstream, ofstream 파일 라이브러리
using namespace std;
#define Width 400
#define Height 300
#define WH 120000
int main()
{
    int i, j, m, individual_idx, compositive_idx;
    ofstream fff("video.rgb", ios::out | ios::binary); // 출력 파일 선언.바이너리
    unsigned char R[WH], G[WH], B[WH], RGB[3 * WH];    // 이미지 저장 공간
    for (m = 100; m < 200; m += 5)
    { // 동영상 만들 때
        //전체 노랗게
        for (i = 0; i < WH; i++)
        {
            R[i] = G[i] = 255;
            *(B + i) = 0;
        }
        // 가운데 빨간 사각형
        for (i = 80; i < 100; i++)
            for (j = 0; j < 40; j++)
            {
                individual_idx = i * 400 + j + m;
                R[individual_idx] = 255;
                *(G + individual_idx) = *(B + individual_idx) = 0;
            }
        // 파일에 RGB 묶어서 넣기 (interlaced)
        for (i = compositive_idx = 0; i < WH; i++, compositive_idx += 3)
        {
            RGB[compositive_idx] = R[i];
            RGB[compositive_idx + 1] = G[i];
            RGB[compositive_idx + 2] = B[i];
        }
        fff.write((const char *)RGB, 3 * WH); // 동영상의 한 프레임 저장
    }
    fff.close();
    return 0;
}