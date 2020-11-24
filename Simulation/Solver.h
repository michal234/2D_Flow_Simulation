#pragma once

#include "Headers.h"
#include "Cell.h"
#include "BinaryMap.h"

using namespace std;

class Solver
{
	private:
		bool initialized;
		bool boundaryConditions;
		int cellGridRows;
		int cellGridCols;
		vector<vector<Cell>> CellGrid;
		vector<Cell> FluidCells;
		double v_start;

		void SetNeighbours();
		int TypeOfNeighbourhood(Cell top, Cell right, Cell bottom, Cell left);
		void UpdateGrid();

	public:
		Solver();
		bool GetInitialized();
		bool GetBoudaryConditions();
		void CellGridInitialization(BinaryMap bm);
		void SetBoundaryConditions(double v);
		void Simulate();
};

