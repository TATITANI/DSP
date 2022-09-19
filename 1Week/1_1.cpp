#include <iostream> // cout, cin
#include <fstream> // ifstream, ofstream 입출력 파일 라이브러리
#include <math.h>

using namespace std;
#define PHI 3.141592

int main() {

	ofstream outFile; // 출력 파일 선언.
	outFile.open("data1_60.txt", ios::out); // 출력 파일 data.txt 열기.

	//최대 주파수가 30hz이므로 샘플링 주파수는 60hz이상
	float t = 0, fs = 60., dt = 1. / fs; // 시간, 샘플링 주파수, 샘플링 주기
	int f0 = 10, n = 6, smp_cnt; // 기본주파수, n개 주기 파형, 신호 샘플 개수
	smp_cnt = (fs / f0) * n;

	//n = nTs이므로 시간 t에 루프마다 샘플링 주기를 더해준다.
	for (int i = 0; i <= smp_cnt; i++, t += dt)
		//샘플링 데이터를 텍스트에 기록한다.
		outFile << t << "\t " <<
		2 + 4 * cos(30 * PHI * t - 0.2 * PHI)
		+ 3 * sin(40 * PHI * t) + 4 * cos(60 * PHI * t - 1 / 3 * PHI)
		<< endl;

	outFile.close();
	return 0;
}
