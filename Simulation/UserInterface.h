#pragma once

#include "Headers.h"
#include "BinaryMap.h"

class UserInterface
{
	private:
		BinaryMap bm;

		void ReadImage();
		void ShowImage();
		void CloseInterface();

	public:
		UserInterface();
		void ShowInterface();
		bool ChoiceService(int choice);
};

