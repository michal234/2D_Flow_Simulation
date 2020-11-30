#include "Results.h"

Results::Results()
{

}

Results::Results(vector<Cell> cg, int rows, int cols)
{
	CellGrid = cg;
	results = Mat::zeros(rows, cols, 16);
}

void Results::ShowVelocityContour()
{
	cout << results.channels() << endl;

	double min = FindMinimumVelocity();
	double max = FindMaximumVelocity();
	double x = 0.0;
	
	for (int i = 0; i < results.rows; i++)
	{
		for (int j = 0; j < results.cols; j++)
		{
			if (!CellGrid[i * results.cols + j].GetFluid())
			{
				Vec3b v;
				v[0] = 0;
				v[1] = 0;
				v[2] = 0;
				results.at<Vec3b>(i, j) = v;
			}
			else
			{
				x = ( CellGrid[i * results.cols + j].GetVelocity() - min) / (max - min);
				Vec3b v = SetColour(x);
				results.at<Vec3b>(i, j) = v;
			}
		}
	}

	namedWindow("Velocity", WINDOW_NORMAL);
	imshow("Velocity", results);
	waitKey(0);
}

double Results::FindMinimumVelocity()
{
	double min = DBL_MAX;
	for (int i = 0; i < CellGrid.size(); i++)
	{
		if( CellGrid[i].GetFluid() && CellGrid[i].GetVelocity() < min )
			min = CellGrid[i].GetVelocity();
	}
	return min;
}

double Results::FindMaximumVelocity()
{
	double max = DBL_MIN;
	for (int i = 0; i < CellGrid.size(); i++)
	{
		if (CellGrid[i].GetFluid() && CellGrid[i].GetVelocity() > max)
			max = CellGrid[i].GetVelocity();
	}
	return max;
}

Vec3b Results::SetColour(double x)
{
	Vec3b v;
	if (x <= 0.5)
	{
		v[0] = floor(255 * (0.5 - x) / 0.5);
		v[1] = floor(255 * x / 0.5);
		v[2] = 0;
	}
	else
	{
		v[0] = 0;
		v[1] = floor(255 * (1.0 - x) / 0.5);
		v[2] = floor(255 * (x - 0.5) / 0.5);
	}
	return v;
}
