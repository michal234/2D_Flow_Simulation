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
		void OpenSolver();
		void SolverInitialization();
		void SolverSimulation();
		void SetBoundaryConditions();
		void CloseSolver();

	public:
		UserInterface();
		void ShowInterface();
		void ShowSolverInterface();
		int ChooseService(int choice);
		int ChooseSolverService(int choice);
};

