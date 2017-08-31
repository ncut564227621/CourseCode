#include"stdafx.h"
#include "MeanShift.h"
#include<ctime>

void CMeanshift::generateDataSet(const int dataNum, const int gaussMeanX, const int gaussMeanY, const int nSigmaX, const int nSigmaY)
{
#define SAVEDATASET
	RNG rngx(0xFFFFFFFF);
	RNG rngy((unsigned)time(NULL));

	int  XMax = 0;
	int  YMax = 0;
	for(int i =0; i<dataNum; i++)
	{
		
		int Point_x = gaussMeanX+ (int)(rngx.gaussian(nSigmaX));
		if(Point_x<0)
			Point_x =0;
		int Point_y = gaussMeanX+ (int)(rngy.gaussian(nSigmaY));
		if(Point_y<0)
			Point_y =0;
		if(XMax<Point_x)
			XMax = Point_x;
		if(YMax<Point_y)
			YMax = Point_y;
		Point data(Point_x, Point_y);
		data_set.push_back(data);
	}
	// plot data set int the map
#ifdef SAVEDATASET
	int nCols = XMax*2;
	int nRows = YMax*2;
	Mat datasetMat(nRows, nCols, CV_8UC3, Scalar::all(0));
	//Vec3b color(0,255,0);
	for(int i =0; i<dataNum; i++)
	{
		datasetMat.at<Vec3b>(data_set[i].y, data_set[i].x)[0] = 0;
		datasetMat.at<Vec3b>(data_set[i].y, data_set[i].x)[1] = 255;
		datasetMat.at<Vec3b>(data_set[i].y, data_set[i].x)[2] = 0;
	}
	//circle(datasetMat, Point(100,99), 3, Scalar(0,0,255),2);
	imwrite("..\\sample\\meanshift_Dataset.bmp", datasetMat);
#endif
	

}
void CMeanshift::process()
{
	int iterNum =0;
	Point2d lastShiftVec(0,0);
	while(iterNum<MAXITERNUM)
	{
		Point2d shiftVec(0,0);
		double total_weight =0.0;
		//Point total_shift =0.0;

		//计算shift vec
		for(int i =0; i<data_set.size(); i++)
		{
			double distance = calDistance(cent_data, data_set[i]);
			double dWeight = 1;// gaussKernel(distance);
			total_weight+=dWeight;
			shiftVec.x += (dWeight*(data_set[i].x - cent_data.x));
			shiftVec.y += (dWeight*(data_set[i].y - cent_data.y));
			//total_shift+=distance*dWeight;
		}
		shiftVec.x = shiftVec.x/total_weight;
		shiftVec.y = shiftVec.y/total_weight;

		//
		cent_data.x = cent_data.x+ shiftVec.x;
		cent_data.y = cent_data.y +shiftVec.y;
		if(fabs(shiftVec.x)<EPSION&&fabs(shiftVec.y)<EPSION)
		{
			cout<<"第"<<iterNum<<"次迭代   center("<<cent_data.x<<","<<cent_data.y<<")"<<"  满足条件 结束迭代"<<endl;
			break;
		}
		lastShiftVec = shiftVec;
		cout<<"第"<<iterNum<<"次迭代: shiftVec("<<shiftVec.x<<","<<shiftVec.y<<")   "<<"center("<<cent_data.x<<","<<cent_data.y<<")"<<endl;
		iterNum++;
	}
}
double CMeanshift::calDistance(Point center, Point sample)
{
	double distance = pow((center.x - sample.x), 2)+ pow((center.y - sample.y),1);
	return sqrt(distance);
}
double CMeanshift::gaussKernel(double distance)
{
	return exp(-(distance*distance)/gaussBandWidth);
}