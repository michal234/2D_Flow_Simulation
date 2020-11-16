#pragma once

#include "Headers.h"
#include "Cell.h"
#include "BinaryMap.h"

using namespace std;

class Solver
{
	private:
		bool initialized;
		vector<vector<Cell>> CellGrid;


	public:
		Solver();
		bool GetInitialized();
		void CellGridInitialization(BinaryMap bm);
};

