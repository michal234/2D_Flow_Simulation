#include "BinaryMap.h"
#include "UserInterface.h"

using namespace std;

int main()
{
    UserInterface ui = UserInterface();
	int choice;
	int chooseInterface;
	bool showNormalInterface = true;
	bool closeInterface = false;
	bool showSolverInterface = false;
	bool closeSolverInterface = false;
	
	while (!closeInterface)
	{
		if(showNormalInterface)
		{
			ui.ShowInterface();
			cin >> choice;
			chooseInterface = ui.ChooseService(choice);
			switch (chooseInterface)
			{
				case 0:
					closeInterface = true;
					break;
				case 1:
					break;	//interface is still opened
				case 2:
					showNormalInterface = false;
					showSolverInterface = true;
					closeSolverInterface = false;
					break;
			}
		}
		if (showSolverInterface)
		{
			while(!closeSolverInterface)
			{
				ui.ShowSolverInterface();
				cin >> choice;
				chooseInterface = ui.ChooseSolverService(choice);
				switch (chooseInterface)
				{
					case 0:
						closeSolverInterface = true;
						showNormalInterface = true;
						showSolverInterface = false;
						break;
					case 1:
						break;	//interface is still opened
				}
			}
		}
		
	}
	return 0;
}
