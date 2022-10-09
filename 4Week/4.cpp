#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>  // sort
#define WORD unsigned short
#define DWORD unsigned int
#define NN 4096
#define N 1024
#define PI 3.141592

// 음 정보 
struct Note {
    float startSecond; // 시작 시간
    float endSecond; // 종료 시간
    int frequency; // 주파수
};

int main() {
    // 1. Read the image file.
    char *f = new char[48];
    FILE *fff, *fout, *foutr;
    if ((fff = fopen("Beatles-LetItBe-wav.wav", "rb")) == NULL) {
        printf("cant open Beatles-LetItBe-wav.wav.\n");
        exit(123);
    }
    // 2. Read the first 48 bytes
    if ((fread(f, 1, 48, fff) != 48)) {
        printf("cant read Beatles-LetItBe-wav.wav..\n");
        exit(444);
    }
    if ((fout = fopen("Beatles-LetItBe-xxx.wav", "wb")) == NULL) {
        printf("cant open Beatles-LetItBe-xxx.wav.\n");
        exit(123);
    }

    // 3. Read all fields.
    printf("RIFF %c%c%c%c\n", f[0], f[1], f[2], f[3]);
    printf("filesize %d\n", *(DWORD *)(f + 4));  // sub-chunk 2 size
    printf("WAVE %c%c%c%c\n", f[8], f[9], f[10], f[11]);
    printf("cksize %d\n", *(DWORD *)(f + 16));
    printf("channels %d\n", *(WORD *)(f + 22));
    printf("fs %d\n", *(DWORD *)(f + 24));
    printf("bytes/s %d\n", *(DWORD *)(f + 28));
    printf("bits/sam %d\n", *(WORD *)(f + 34));
    printf("data %c%c%c%c\n", f[36], f[37], f[38], f[39]);
    printf("cksize %d\n", *(DWORD *)(f + 40));  // sub-chunk 2 size
    DWORD FileSize = *(DWORD *)(f + 40);

    if ((fwrite(f, 1, 48, fout) != 48)) {
        printf("cant write header on Beatles-LetItBe-xxx.wav. \n");
    }

    FileSize /= 4;  // 4bytes per sample

    // 4. modify audio signal to mono.
    const int fs = *(DWORD *)(f + 24);
    const float dt = 1. / (float)(fs);
    //음표 정보
    Note notes[] = {{3.f, 4.5f, 784}, {5.f, 5.5f, 659}, {6.f, 6.5f, 659},
                    {7.f, 7.5f, 784}, {8.f, 8.5f, 659}, {9.f, 10.5f, 523}};

    int noteID = 0;
    const int notesCnt = sizeof(notes) / sizeof(Note);
    //  음표 등장 시간순 정렬
    std::sort(notes, notes + notesCnt,
              [](Note a, Note b) { return a.startSecond < b.startSecond; });

    short leftdata[N], rightdata[N], alldata[N * 2];
    for (int n = 0; n < FileSize; n += N) {  // all data
        if ((fread(alldata, 4, N, fff)) != N) {
            printf("cant read %d-th data from Beatles-LetItBe-wav.wav. \n", n);
            // exit(244);
        }
        // store left/right data
        int ii, iii;
        for (ii = iii = 0; ii < N; ii++, iii += 2) {
            leftdata[ii] = alldata[iii];
            rightdata[ii] = alldata[iii + 1];
        }

        // modify left/right data
        for (int i = 0; i < N; i++) {
            float currentTime = (float)(n + i) / fs;

            // 곡 삽입구간 데이터 초기화
            if (currentTime > notes[0].startSecond &&
                currentTime < notes[notesCnt - 1].endSecond) {
                leftdata[i] = rightdata[i] = 0;
            }

            //현재 음 갱신
            if (currentTime > notes[noteID].endSecond && noteID < notesCnt) {
                noteID++;
            }
            //현재 음 입력
            if (currentTime > notes[noteID].startSecond &&
                currentTime < notes[noteID].endSecond) {
                float t = i * dt;
                leftdata[i] = rightdata[i] =
                    (short)(2000 * sin(2.f * PI * notes[noteID].frequency * t));
            }
        }

        // restore left/right data
        for (ii = iii = 0; ii < N; ii++, iii += 2) {
            alldata[iii] = leftdata[ii];
            alldata[iii + 1] = rightdata[ii];
        }

        // 5. write on output file
        if (fwrite(alldata, 4, N, fout) != N) {
            printf("cant write %d-th data on Beatles-LetItBe-xxx.wav. \n", n);
        }
    }  // all data

    fclose(fff);
    fclose(fout);
    delete f;

    return 1;
}
