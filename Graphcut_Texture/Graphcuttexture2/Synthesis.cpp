#include "Synthesis.h"
#include "graph.h"
#include "math.h"

#define INFINITY ((int)(((unsigned)-1)/2))

Synthesis::Synthesis(void){

}

Synthesis::~Synthesis(void){

}

double Synthesis::cost(int s, int t,int n){  //算出结点s和结点t之间边的代价(权重)
	double costs = 0;
	double costt = 0;
	int swidth = s / overlapheight[n];
	int sheight = s%overlapheight[n];
	int twidth = t / overlapheight[n];
	int theight = t%overlapheight[n];
	int i;
	double totalcost;
	for (i = 0; i < 3; i++){
		costs += ((double)(patch.at<Vec3b>(sheight, swidth)[i] - background.at<Vec3b>(sheight + background.rows - overlapheight[n], swidth + background.cols - overlapwidth[n])[i]))*((double)(patch.at<Vec3b>(sheight, swidth)[i] - background.at<Vec3b>(sheight + background.rows - overlapheight[n], swidth + background.cols - overlapwidth[n])[i]));
		costt += ((double)(patch.at<Vec3b>(theight, twidth)[i] - background.at<Vec3b>(theight + background.rows - overlapheight[n], twidth + background.cols - overlapwidth[n])[i]))*((double)(patch.at<Vec3b>(theight, twidth)[i] - background.at<Vec3b>(theight + background.rows - overlapheight[n], twidth + background.cols - overlapwidth[n])[i]));
	}
	totalcost = sqrt(costs) + sqrt(costt);
	return totalcost;
}

void Synthesis::texturesynthesis(){  //利用最大流最小割合成图
	for (int n = 0; n < 2; n++){
		int i, j, flow;
		int nodenum = overlapheight[n]*overlapwidth[n];
		int edgenum = (overlapheight[n] - 1)*overlapwidth[n] + (overlapwidth[n] - 1)*overlapheight[n];
		typedef Graph<int, int, int> GraphType;
		GraphType *G = new GraphType(nodenum, edgenum);
		for (i = 0; i < nodenum; i++){
			G->add_node();
		}
		for (i = 0; i<overlapheight[n]; i++){
			G->add_tweights(i, INFINITY, 0);
			G->add_tweights(overlapheight[n]*(overlapwidth[n] - 1) + i, 0, INFINITY);
		}
		for (i = 0; i<overlapwidth[n]; i++){
			G->add_tweights(i*overlapheight[n], INFINITY, 0);
			G->add_tweights(i*overlapheight[n] + overlapheight[n] - 1, 0, INFINITY);
		}
		for (i = 0; i<overlapheight[n]; i++){
			for (j = 0; j<overlapwidth[n] - 1; j++){
				G->add_edge(j*overlapheight[n] + i, (j + 1)*overlapheight[n] + i, cost(j*overlapheight[n] + i, (j + 1)*overlapheight[n] + i,n), 0);
			}
		}
		for (i = 0; i<overlapwidth[n]; i++){
			for (j = 0; j<overlapheight[n] - 1; j++){
				G->add_edge(i*overlapheight[n] + j, i*overlapheight[n] + j + 1, cost(i*overlapheight[n] + j, i*overlapheight[n] + j + 1,n), 0);
			}
		}
		flow = G->maxflow();
		for (i = 0; i < nodenum; i++){
			if (G->what_segment(i) == GraphType::SOURCE){
				cutmark.push_back(1);
			}
			else{
				cutmark.push_back(0);
			}
		}
		Mat outputimage = Mat::zeros(patch.rows+background.rows - overlapheight[n], patch.cols+background.cols - overlapwidth[n], 16);
		for (i = 0; i < background.cols; i++){
			for (j = 0; j < background.rows; j++){
				outputimage.at<Vec3b>(j, i) = background.at<Vec3b>(j, i);
			}
		}
		for (i = 0; i < patch.cols; i++){
			for (j = 0; j < patch.rows; j++){
				outputimage.at<Vec3b>(j + background.rows - overlapheight[n], i + background.cols - overlapwidth[n]) = patch.at<Vec3b>(j, i);
			}
		}
		for (i = 0; i<nodenum; i++){
			if (cutmark[i] == 1){
				outputimage.at<Vec3b>(i%overlapheight[n] + background.rows - overlapheight[n], i / overlapheight[n] + background.cols - overlapwidth[n]) = background.at<Vec3b>(i%overlapheight[n] + background.rows - overlapheight[n], i / overlapheight[n] + background.cols - overlapwidth[n]);
			}
		}
		delete G;
		cutmark.clear();
		background = outputimage;
		imshow("output", outputimage);
		waitKey(0);
	}
}