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
	cout << "3 - solver\n";
	cout << "4 - zakoncz dzialanie programu\n";
	cout << ">>";
}

bool UserInterface::ChoiceService(int choice)
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
			SolveSimulation();
			break;
		case 4:
			CloseInterface();
			return false;
		default:
			break;
	}
	return true;
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

void UserInterface::SolveSimulation()
{
	solver = Solver(bm);
}