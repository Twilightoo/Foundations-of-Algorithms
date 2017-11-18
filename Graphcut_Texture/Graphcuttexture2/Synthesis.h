#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

class Synthesis{
public:
	Synthesis(void);
	~Synthesis(void);
	Synthesis(Mat& image, int x1, int y1,int x2,int y2){
		patch = image;
		overlapwidth[0] = x1;
		overlapheight[0] = y1;
		overlapwidth[1] = x2;
		overlapheight[1] = y2;
		background = image;
	}
	double cost(int s, int t,int i);
	void texturesynthesis();
private:
	int overlapwidth[2];
	int overlapheight[2];
	Mat patch;
	Mat background;
	vector<int> cutmark;
};