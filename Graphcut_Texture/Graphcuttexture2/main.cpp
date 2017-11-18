#include "Synthesis.h"
#include "time.h"
#include "iostream"

using namespace std;

int main(){
	string s;
	cout << "choose an image: texture1.jpg  texture2.jpg  texture3.jpg  texture4.jpg" << endl;
	cin >> s;
	Mat image = imread(s);
	int x1, y1, x2, y2;
	srand((unsigned)time(NULL));  //随机生成图像放置的位置
    x1 = rand() % image.cols;
	y1 = rand() % image.rows;
	x2 = rand() % image.cols;
	y2 = rand() % image.rows;
	Synthesis* generator = new Synthesis(image, x1, y1, x2, y2);
	generator->texturesynthesis();
}