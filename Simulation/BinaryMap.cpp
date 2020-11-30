#include "BinaryMap.h"

using namespace cv;
using namespace std;

BinaryMap::BinaryMap()
{
	//input = Mat::zeros(0, 0, 0);
}


BinaryMap::BinaryMap(const string path)
{
	Mat img = imread(path);
	//cout << img.type() << endl;
	if( img.channels() > 1 )
		input = ConvertToOneChannel(img);
	else
		input = img;
}

bool BinaryMap::IsEmpty()
{
	return input.empty();
}

void BinaryMap::ShowMap()
{
	namedWindow("Mapa osrodka", WINDOW_NORMAL);
	imshow("Mapa osrodka", input);
	waitKey(0);
}

Mat BinaryMap::ConvertToOneChannel(Mat src)
{
	Mat res = Mat::zeros(src.rows, src.cols, 0);
	for( int i = 0; i < src.rows; i++ )
		for (int j = 0; j < src.cols; j++)
			res.at<uchar>(i, j) = src.at<Vec3b>(i, j)[0];

	return res;
}

int BinaryMap::GetRows()
{
	return input.rows;
}

int BinaryMap::GetCols()
{
	return input.cols;
}

int BinaryMap::GetElement(int i, int j)
{
	return input.at<uchar>(i, j);
}