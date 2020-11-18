#pragma once

#include "Headers.h"
#include "Cell.h"
#include "BinaryMap.h"

using namespace std;

class Solver
{
	private:
		bool initialized;
		int cellGridRows;
		int cellGridCols;
		vector<vector<Cell>> CellGrid;
		vector<Cell> FluidCells;

		void SetNeighbours();
		int TypeOfNeighbourhood(Cell top, Cell right, Cell bottom, Cell left);

	public:
		Solver();
		bool GetInitialized();
		void CellGridInitialization(BinaryMap bm);
		void Simulate();
};

