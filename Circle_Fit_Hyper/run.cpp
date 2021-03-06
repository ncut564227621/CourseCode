#include "stdafx.h"
#include"opencv2/opencv.hpp"

#include "CircleFitByHyper.h"


void randomlineData(Mat& fitMap, vector<Point>& fitData, vector<Point>& fitData_2, const int ptNums);


int _tmain(int argc, _TCHAR* argv[])
{
	Mat img = imread("..\\Sample\\coins.bmp",CV_LOAD_IMAGE_COLOR);//image size:720*580 px

	Mat _inImg(img.size(), CV_8UC1, Scalar(0));
	Mat _outImg(img.size(), CV_8UC1, Scalar(0));

	if(img.empty())
	{
		cout<<"input img is wrong!"<<endl;
		return -1;
	}
	if(img.channels() == 3)
	{
		cvtColor(img, _inImg, CV_RGB2GRAY);
	}
	else
	{
		  _inImg = img.clone();
	}
	

	Mat plotMat(1024,1024, CV_8UC3, Scalar::all(10));
	Vec3f circleParam(512,512,300);
	vector<Point>circleData;
	int dataNums = 1800;
	randomCircleData(plotMat, circleParam, circleData, dataNums);

	Circle FitCircle;
	unsigned short int NrOfPoints = 0;
	reals *XiData, *YiData = NULL;

	NrOfPoints = circleData.size();

	XiData = new reals[NrOfPoints];
	YiData = new reals[NrOfPoints];

	for(int i=0; i<NrOfPoints; i++)
	{
		XiData[i] = (double)circleData[i].x;
		YiData[i] = (double)circleData[i].y;
	}


	Data data1(NrOfPoints, XiData, YiData);

	

	
	const int64 nStart = getTickCount();
	//circleFit(plotMat, circleData);
	FitCircle = CircleFitByHyper(data1);
	double nDuration = (getTickCount() - nStart)/getTickFrequency();
	cout<<"cost time: "<<nDuration*1000<<"ms"<<endl;

	Point2f center = Point2f(FitCircle.a,FitCircle.b);
	int nRadius = FitCircle.r;

	//int nRadius = (int)(sz.width+sz.height)>>2;

	circle(plotMat, center, nRadius, Scalar(0,0,255));
	imwrite("..\\sample\\result\\circle_data_fit.bmp", plotMat);

	//imwrite("..\\Sample\\result\\ransac_circle.bmp", img);
	int a =0;
	

	

	return 0;
}

void randomlineData(Mat& fitMap, vector<Point>& fitData_1,vector<Point>& fitData_2, const int ptNums)
{
	if(fitMap.empty())
	{
		cout<<"fitData Mat is empty!"<<endl;
		return;
	}
	int a_1 = 2;
	int b_1 = 10;

	int a_2 = -1;
	int b_2 = 350;
	RNG rng(0xFFFFFFFF);
	RNG rng_eps(0xFFFFFFFF);

	for(int i =0; i<ptNums; i++)
	{
		Point pt_1;
		Point pt_2;
		
		pt_2.x = rng.uniform(3, fitMap.cols-3);
		pt_2.y = a_2*pt_2.x+b_2+rng_eps.gaussian(7);
		if(i%10 == 0)
			pt_2.y = a_2*(pt_2.x-rng_eps.gaussian(15))+b_2+rng_eps.gaussian(50);
		if(pt_2.y<=fitMap.rows-1&& pt_2.y>=0)
		{
			fitData_2.push_back(pt_2);
			fitMap.at<Vec3b>(pt_2.y,pt_2.x)[0] = 255;
			fitMap.at<Vec3b>(pt_2.y,pt_2.x)[1] = 255;
			fitMap.at<Vec3b>(pt_2.y,pt_2.x)[2] = 0;
		}	

		pt_1.x = rng.uniform(3, fitMap.cols-3);
		pt_1.y = a_1*pt_1.x+b_1+rng_eps.gaussian(7);
		if(i%5 == 0)
			pt_1.y = 0.5*a_2*(pt_1.x-rng_eps.gaussian(15))+b_2-rng_eps.gaussian(35);

		if(pt_1.y<=fitMap.rows-1&& pt_1.y>=0)
		{
			fitData_1.push_back(pt_1);
			fitMap.at<Vec3b>(pt_1.y,pt_1.x)[0] = 0;
			fitMap.at<Vec3b>(pt_1.y,pt_1.x)[1] = 0;
			fitMap.at<Vec3b>(pt_1.y,pt_1.x)[2] = 255;
		}	
	}
}