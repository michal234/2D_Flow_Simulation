#pragma once

#include "Headers.h"

using namespace std;

class Cell
{
	private:
		bool fluid;		//if cell is fluid it's true
		bool wall;		//if cell is fluid and lays next to the solid it's true
		bool boundary;	//if cell lays on X=0 or X=N-1 or Y=0 or Y=M-1 it's true

		int x;	//X-coordinate of the cell
		int y;	//Y-coordinate of the cell

		map<string, Cell> neighbours;	//neighbours of the cell

	public:
		Cell();
		Cell(bool fluid, bool boundary, int x, int y);
};

