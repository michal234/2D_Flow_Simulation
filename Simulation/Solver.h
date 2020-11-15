#pragma once

#include "Headers.h"
#include "Cell.h"
#include "BinaryMap.h"

using namespace std;

class Solver
{
	private:
		vector<vector<Cell>> CellGrid;


	public:
		Solver();
		Solver(BinaryMap bm);
};

