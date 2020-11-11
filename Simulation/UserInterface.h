#pragma once

#include "Headers.h"
#include "BinaryMap.h"
#include "Cell.h"

using namespace std;

class UserInterface
{
	private:
		BinaryMap bm;
		vector<vector<Cell>> CellGrid;

		void ReadImage();
		void ShowImage();
		void CloseInterface();
		void InitializeCellularAutomata();

	public:
		UserInterface();
		void ShowInterface();
		bool ChoiceService(int choice);
};

