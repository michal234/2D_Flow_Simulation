#include "BinaryMap.h"
#include "UserInterface.h"

using namespace std;

int main()
{
    UserInterface ui = UserInterface();
	int choice;
	while (true)
	{
		ui.ShowInterface();
		cin >> choice;
		bool showInterface = ui.ChoiceService(choice);
		if( !showInterface )
			break;
	}
	return 0;
}
