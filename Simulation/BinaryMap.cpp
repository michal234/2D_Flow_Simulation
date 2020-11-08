#include "BinaryMap.h"

using namespace cv;
using namespace std;

BinaryMap::BinaryMap()
{
	//input = Mat::zeros(0, 0, 0);
}


BinaryMap::BinaryMap(const string path)
{
	input = imread(path);
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