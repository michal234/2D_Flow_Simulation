#include "Cell.h"

Cell::Cell()
{

}

Cell::Cell(bool fluid, bool boundary, int x, int y)
{
	this->fluid = fluid;
	this->boundary = boundary;
	this->x = x;
	this->y = y;
}