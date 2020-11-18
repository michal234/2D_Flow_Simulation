#include "Cell.h"

Cell::Cell()
{

}

Cell::Cell(bool fluid)
{
	this->fluid = fluid;
}

Cell::Cell(bool fluid, bool boundary, int x, int y)
{
	this->fluid = fluid;
	this->boundary = boundary;
	this->x = x;
	this->y = y;
}

bool Cell::GetFluid()
{
	return fluid;
}

bool Cell::GetBoundary()
{
	return boundary;
}

int Cell::GetX()
{
	return x;
}

int Cell::GetY()
{
	return y;
}

void Cell::SetNeighbours(map<string, Cell> neighbours)
{
	this->neighbours = neighbours;
}

void Cell::SetTypeOfNeighbourhood(int type)
{
	this->typeOfNeighbourhood = type;
}