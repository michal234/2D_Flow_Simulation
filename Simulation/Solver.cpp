#include "Solver.h"

Solver::Solver()
{
	initialized = false;
}

bool Solver::GetInitialized()
{
	return initialized;
}

void Solver::CellGridInitialization(BinaryMap bm)
{
	int rows = bm.GetRows();
	int cols = bm.GetCols();
	for (int i = 0; i < rows; i++)
	{
		vector<Cell> vc;
		for (int j = 0; j < cols; j++)
		{
			int element = bm.GetElement(i, j);
			bool fluid = true;
			bool boundary = false;
			if (element == 0)	//solid
				fluid = false;
			if( i==0 || i == rows-1 || j ==0 || j==cols-1 )
				boundary = true;
			Cell cell(fluid, boundary, i, j);
			vc.push_back(cell);
			
		}
		CellGrid.push_back(vc);
	}

	initialized = true;
}