#pragma once

#include "Headers.h"

using namespace std;
using namespace cv;

class BinaryMap
{
	private:
	Mat input;

	public:
	BinaryMap();
	BinaryMap(const string path);
	bool IsEmpty();
	void ShowMap();
};

