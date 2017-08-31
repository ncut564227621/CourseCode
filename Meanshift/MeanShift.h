#pragma  once
#include"opencv2/opencv.hpp"
#include<iostream>
#include"ippi.h"
#include<vector>

using namespace cv;
using namespace std;


#ifdef TESTALGORITHMS_EXPORTS
#define TESTALGORITHMS_API __declspec(dllexport)
#else
#define TESTALGORITHMS_API __declspec(dllimport)
#endif



class  TESTALGORITHMS_API CMeanshift
{
public:
	Point cent_data;
	vector<Point> data_set;

    #define EPSION 0.0001
    #define MAXITERNUM 10000

	double gaussBandWidth;

	//
     double gaussKernel(double distance);
     void process();

	 double calDistance(Point center, Point sample);
	 //产生2D数据集接口
	 
	 void generateDataSet(const int dataNum, const int gaussMeanX, const int gaussMeanY, const int nSigmaX, const int nSigmaY);
};
