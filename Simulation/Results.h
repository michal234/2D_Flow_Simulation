#pragma once

#include "Headers.h"
#include "Cell.h"

using namespace std;
using namespace cv;

class Results
{
	private:
		vector<Cell> CellGrid;
		Mat results;

		double FindMinimumVelocity();
		double FindMaximumVelocity();
		Vec3b SetColour(double x);

	public:
		Results();
		Results(vector<Cell> cg, int rows, int cols);
		void ShowVelocityContour();
};

