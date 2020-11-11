#pragma once

#include "Headers.h"

using namespace std;
using namespace cv;

class BinaryMap
{
	private:
		Mat input;
		Mat ConvertToOneChannel(Mat src);

	public:
		BinaryMap();
		BinaryMap(const string path);
		bool IsEmpty();
		void ShowMap();
		int GetRows();
		int GetCols();
		int GetElement(int i, int j);
	
};

