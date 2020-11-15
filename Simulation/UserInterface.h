#pragma once

#include "Headers.h"
#include "BinaryMap.h"
#include "Cell.h"
#include "Solver.h"

using namespace std;

class UserInterface
{
	private:
		BinaryMap bm;
		Solver solver;

		void ReadImage();
		void ShowImage();
		void CloseInterface();
		void SolveSimulation();

	public:
		UserInterface();
		void ShowInterface();
		bool ChoiceService(int choice);
};

