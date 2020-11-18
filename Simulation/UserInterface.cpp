#include "UserInterface.h"

using namespace std;

UserInterface::UserInterface()
{
	bm = BinaryMap();
}

void UserInterface::ShowInterface()
{
	cout << "Wybierz opcje:\n";
	cout << "1 - wczytaj obraz\n";
	cout << "2 - wyswietl obraz\n";
	cout << "3 - otworz solver\n";
	cout << "4 - zakoncz dzialanie programu\n";
	cout << ">>";
}

int UserInterface::ChooseService(int choice)
{
	switch (choice)
	{
		case 1:
			ReadImage();
			break;
		case 2:
			ShowImage();
			break;
		case 3:
			if (bm.IsEmpty())
			{
				cout << "Nie wczytano obrazu!\n";
				break;
			}
			OpenSolver();
			return 2;
		case 4:
			CloseInterface();
			return 0;
		default:
			break;
	}
	return 1;
}

void UserInterface::ShowSolverInterface()
{
	cout << "\nSOLVER\n";
	cout << "Wybierz opcje:\n";
	cout << "1 - inicjalizacja sieci automatow komorkowych\n";
	cout << "2 - warunki brzegowe\n";
	cout << "3 - obliczenie symulacji\n";
	cout << "4 - zamknij solver\n";
	cout << ">>";
}

int UserInterface::ChooseSolverService(int choice)
{
	switch (choice)
	{
		case 1:
			SolverInitialization();
			break;
		case 2:
		
			break;
		case 3:
			SolverSimulation();
			break;
		case 4:
			CloseSolver();
			return 0;
		default:
			break;
	}
	return 1;
}

void UserInterface::ReadImage()
{
	cout << "Podaj sciezke dostepu do obrazu: ";
	string path;
	cin >> path;
	bm = BinaryMap(path);
	cout << "Obraz zostal wczytany\n";

}

void UserInterface::ShowImage()
{
	if (bm.IsEmpty())
	{
		cout << "Nie wczytano obrazu!\n";
		return;
	}
	bm.ShowMap();
}

void UserInterface::CloseInterface()
{
	cout << "Interfejs zostal zamkniety\n";
}

void UserInterface::OpenSolver()
{
	solver = Solver();
	cout << "Solver zostal otwarty pomyslnie\n";
}

void UserInterface::SolverInitialization()
{
	solver.CellGridInitialization(bm);
	//cout << "Pomyslnie zainicjalizowano siatke automatow komorkowych\n";
}

void UserInterface::SolverSimulation()
{
	if (!solver.GetInitialized())
	{
		cout << "Siatka niezainicjalizowana!\n";
		return;
	}
	//tu robimy symulacjê
	solver.Simulate();

}

void UserInterface::CloseSolver()
{
	cout << "Solver zostal zamkniety\n";
}
