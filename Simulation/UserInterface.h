#pragma once

#include "Headers.h"
#include "BinaryMap.h"
#include "Cell.h"
#include "Solver.h"
#include "Results.h"

using namespace std;

class UserInterface
{
	private:
		BinaryMap bm;
		Solver solver;
		Results results;

		void ReadImage();
		void ShowImage();
		void CloseInterface();
		void OpenSolver();
		void SolverInitialization();
		void SolverSimulation();
		void SetBoundaryConditions();
		void CloseSolver();
		void ShowResults();

	public:
		UserInterface();
		void ShowInterface();
		void ShowSolverInterface();
		int ChooseService(int choice);
		int ChooseSolverService(int choice);
};

