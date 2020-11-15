#include "Solver.h"

Solver::Solver()
{

}

Solver::Solver(BinaryMap bm)
{
	int rows = bm.GetRows();
	int cols = bm.GetCols();
	for (int i = 0; i < rows; i++)
	{
		vector<Cell> vc;
		for (int j = 0; j < cols; j++)
		{
			int element = bm.GetElement(i, j);
			if (element == 0)	//solid
			{
				Cell cell(0);
				vc.push_back(cell);
			}
			if (element == 255)	//fluid
			{
				Cell cell(1);
				vc.push_back(cell);
			}
		}
		CellGrid.push_back(vc);
	}

	cout << "Pomyslnie zainicjalizowano siatke automatow komorkowych\n";
}