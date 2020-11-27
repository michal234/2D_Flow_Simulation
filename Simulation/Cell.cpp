#include "Cell.h"

Cell::Cell()
{
	input_top = input_right = input_bottom = input_left = 0.0;
	input_top_next = input_right_next = input_bottom_next = input_left_next = 0.0;
	input_total = 0.0;

	output_top = output_right = output_bottom = output_left = 0.0;
	output_top_next = output_right_next = output_bottom_next = output_left_next = 0.0;
	output_total = output_total_next = 0.0;

	fluid_amount = 0.0;

	typeOfNeighbourhood = 0;

	x = y = 0;

	fluid = source = boundary = false;
}

Cell::Cell(bool fluid)
{
	this->fluid = fluid;

	input_top = input_right = input_bottom = input_left = 0.0;
	input_top_next = input_right_next = input_bottom_next = input_left_next = 0.0;
	input_total = 0.0;

	output_top = output_right = output_bottom = output_left = 0.0;
	output_top_next = output_right_next = output_bottom_next = output_left_next = 0.0;
	output_total = output_total_next = 0.0;

	fluid_amount = 0.0;

	typeOfNeighbourhood = 0;

	x = y = 0;

	source = boundary = false;
}

Cell::Cell(bool fluid, bool boundary, int x, int y)
{
	this->fluid = fluid;
	this->boundary = boundary;
	this->x = x;
	this->y = y;

	input_top = input_right = input_bottom = input_left = 0.0;
	input_top_next = input_right_next = input_bottom_next = input_left_next = 0.0;
	input_total = 0.0;

	output_top = output_right = output_bottom = output_left = 0.0;
	output_top_next = output_right_next = output_bottom_next = output_left_next = 0.0;
	output_total = output_total_next = 0.0;

	fluid_amount = 0.0;

	typeOfNeighbourhood = 0;

	source = false;
}

bool Cell::GetFluid()
{
	return fluid;
}

bool Cell::GetBoundary()
{
	return boundary;
}

bool Cell::GetSource()
{
	return source;
}

bool Cell::GetBalance()
{
	if( abs(input_total - output_total) < 1e-5 )
		return true;
	return false;
}

double Cell::GetFluidAmount()
{
	return fluid_amount;
}

/*double Cell::GetInputTop()
{
	return input_top;
}

double Cell::GetInputRight()
{
	return input_right;
}

double Cell::GetInputBottom()
{
	return input_bottom;
}

double Cell::GetInputLeft()
{
	return input_left;
}*/

int Cell::GetX()
{
	return x;
}

int Cell::GetY()
{
	return y;
}

void Cell::SetSource()
{
	source = true;
}

/*void Cell::SetNeighbours(map<string, Cell*> neighbours)
{
	this->neighbours = neighbours;
}*/

void Cell::SetNeighbours(Cell* top, Cell* right, Cell* bottom, Cell* left)
{
	this->neighbours.insert({ "Top", top });
	this->neighbours.insert({ "Right", right });
	this->neighbours.insert({ "Bottom", bottom });
	this->neighbours.insert({ "Left", left });
}

void Cell::SetTypeOfNeighbourhood(int type)
{
	this->typeOfNeighbourhood = type;
}

void Cell::SetNeighboursOnSlant(Cell* topRight, Cell* bottomRight, Cell* bottomLeft, Cell* topLeft)
{
	this->neighboursOnSlant.insert({ "TopRight", topRight });
	this->neighboursOnSlant.insert({ "BottomRight", bottomRight });
	this->neighboursOnSlant.insert({ "BottomLeft", bottomLeft });
	this->neighboursOnSlant.insert({ "TopLeft", topLeft });
}

void Cell::SetTypeOfNeighbourhoodOnSlant(int type)
{
	this->typeOfNeighbourhoodOnSlant = type;
}

/*void Cell::SetOutputTotal(double out)
{
	output_total_next = out;
}*/

void Cell::FluidFlow()
{
	
	//calculate direction of output
	double x_direction = input_left - input_right;
	double y_direction = input_bottom - input_top;

	input_total = input_left + input_bottom + input_right + input_top;

	if (abs(x_direction) < 1e-5 && abs(y_direction) < 1e-5)
	{
		UniformFlow();
		return;
	}
	
	int direction = ChooseDirection(x_direction, y_direction);
	double angle;
	double angle_r;
	if( abs(x_direction) > 1e-5 ) 
		angle_r = atan(y_direction / x_direction);
	else
		angle_r = PI / 2;
	angle = angle_r * 180 / PI;
	double abs_angle = abs(angle);

	double top_flow = 0.0;
	double right_flow = 0.0;
	double bottom_flow = 0.0;
	double left_flow = 0.0;

	double top_right_flow = 0.0;
	double bottom_right_flow = 0.0;
	double bottom_left_flow = 0.0;
	double top_left_flow = 0.0;

	double a = 0.0;

	if (typeOfNeighbourhood == 0)
	{
		switch (typeOfNeighbourhoodOnSlant)
		{
			case 0:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5-a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant( top_right_flow*abs(sin(angle_r)), top_right_flow*cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a - 0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow * cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow * cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a - 0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow * abs(sin(angle_r)), bottom_left_flow * cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow * abs(sin(angle_r)), bottom_left_flow * cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a - 0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow * cos(angle_r), bottom_right_flow * abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow * cos(angle_r), bottom_right_flow * abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 1:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow * abs(sin(angle_r)), bottom_left_flow * cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 2:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;
				

			case 3:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow * cos(angle_r), bottom_right_flow * abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 4:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow * cos(angle_r), bottom_right_flow * abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 5:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow * cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow * abs(sin(angle_r)), bottom_left_flow * cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 6:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow * cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 7:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow * abs(sin(angle_r)), bottom_left_flow * cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 8:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow * cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow * cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 9:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 10:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow * cos(angle_r), bottom_right_flow * abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 11:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_left_flow = input_total * a / 0.5;
						top_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow * abs(sin(angle_r)), 0, 0, top_left_flow * cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						left_flow = input_total * a / 0.5;
						top_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_left_flow* abs(sin(angle_r)), 0, 0, top_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 12:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						top_right_flow = input_total * a / 0.5;
						right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
					else
					{
						a -= 0.5;
						top_flow = input_total * a / 0.5;
						top_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(top_right_flow * abs(sin(angle_r)), top_right_flow * cos(angle_r), 0, 0, direction);
					}
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 13:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					a = (angle + 90) / 90;
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_right_flow = input_total * a / 0.5;
						bottom_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow * cos(angle_r), bottom_right_flow * abs(sin(angle_r)), 0, direction);
					}
					else
					{
						a -= 0.5;
						right_flow = input_total * a / 0.5;
						bottom_right_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, bottom_right_flow* cos(angle_r), bottom_right_flow* abs(sin(angle_r)), 0, direction);
					}
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 14:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					if (a < 0.5 || abs(a-0.5) < 1e-5)
					{
						bottom_left_flow = input_total * a / 0.5;
						left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
					else
					{
						a -= 0.5;
						bottom_flow = input_total * a / 0.5;
						bottom_left_flow = input_total * (0.5 - a) / 0.5;
						FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
						FlowToNeighboursOnSlant(0, 0, bottom_left_flow* abs(sin(angle_r)), bottom_left_flow* cos(angle_r), direction);
					}
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;

			case 15:
				a = abs_angle / 90;
				if (angle > 0 && x_direction > 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction < 0 && y_direction > 0)
				{
					top_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle > 0 && x_direction < 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					left_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (angle < 0 && x_direction > 0 && y_direction < 0)
				{
					bottom_flow = input_total * y_direction / (x_direction + y_direction);
					right_flow = input_total * x_direction / (x_direction + y_direction);
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction > 0)
				{
					top_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (abs(x_direction) < 1e-5 && y_direction < 0)
				{
					bottom_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction > 0 && abs(y_direction) < 1e-5)
				{
					right_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				else if (x_direction < 0 && abs(y_direction) < 1e-5)
				{
					left_flow = input_total;
					FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				}
				break;
		}
	}

	switch (typeOfNeighbourhood)
	{
		case 1:
			if( x_direction >= 0 )
				right_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction) );
			else
				left_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			bottom_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 2:
			if (y_direction >= 0)
				top_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			else
				bottom_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			left_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 3:
			if (x_direction >= 0)
				right_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			else
				left_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			top_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 4:
			if (y_direction >= 0)
				top_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			else
				bottom_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			right_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 5:
			left_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			bottom_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 6:
			if (x_direction >= 0)
				right_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			else
				left_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 7:
			right_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			bottom_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 8:
			left_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			top_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 9:
			if (y_direction >= 0)
				top_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			else
				bottom_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 10:
			right_flow = input_total * abs(y_direction) / (abs(x_direction) + (y_direction));
			top_flow = input_total * abs(x_direction) / (abs(x_direction) + (y_direction));
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 11:
			left_flow = input_total;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 12:
			top_flow = input_total;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 13:
			right_flow = input_total;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 14:
			bottom_flow = input_total;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;
	}

	/*double x_flow = input_total * abs(xd) / (abs(xd) + abs(yd));
	double y_flow = input_total * abs(yd) / (abs(xd) + abs(yd));

	//auxiliary variables
	double xd_new = 0.0;
	double yd_new = 0.0;
	double x_flow_new = 0.0;
	double y_flow_new = 0.0;

	switch (typeOfNeighbourhood)
	{
		case 0:	//(t && r && b && l)
			FlowToNeighbours(xd, yd, x_flow, y_flow);
			break;

		case 1:	//(!t && r && b && l)
			if( yd > 0 )	//if the direction of flow is top
				yd = -yd;	//change the direction to bottom
			FlowToNeighbours(xd, yd, x_flow, y_flow);
			break;

		case 2:	//(t && !r && b && l)
			if ( xd > 0 )	//if the direction of flow is right
				xd = -xd;	//change the direction to left
			FlowToNeighbours(xd, yd, x_flow, y_flow);
			break;

		case 3:	//(t && r && !b && l)
			if (yd < 0)	//if the direction of flow is bottom
				yd = -yd;	//change the direction to top
			FlowToNeighbours(xd, yd, x_flow, y_flow);
			break;

		case 4:	//(t && r && b && !l)
			if (xd < 0)	//if the direction of flow is left
				xd = -xd;	//change the direction to right
			FlowToNeighbours(xd, yd, x_flow, y_flow);
			break;

		case 5:	//(!t && !r && b && l)
			if ( xd > 0 )	//if the direction of flow is right
			{
				yd_new = -xd;
				y_flow_new = x_flow;
				xd = 0.0;
				x_flow = 0.0;
			}
			if ( yd > 0 )	//if the diraction of flow is top
			{
				xd_new = -yd;
				x_flow_new = y_flow;
				yd = 0.0;
				y_flow = 0.0;
			}
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 6:	//(!t && r && !b && l)
			xd_new = xd + yd;
			x_flow_new = x_flow + y_flow;
			yd_new = 0.0;
			y_flow_new = 0.0;
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 7:	//(!t && r && b && !l)
			if (xd < 0)	//if the direction of flow is left
			{
				yd_new = xd;
				y_flow_new = x_flow;
				xd = 0.0;
				x_flow = 0.0;
			}
			if (yd > 0)	//if the diraction of flow is top
			{
				xd_new = yd;
				x_flow_new = y_flow;
				yd = 0.0;
				y_flow = 0.0;
			}
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 8:	//(t && !r && !b && l)
			if (xd > 0)	//if the direction of flow is right
			{
				yd_new = xd;
				y_flow_new = x_flow;
				xd = 0.0;
				x_flow = 0.0;
			}
			if (yd < 0)	//if the diraction of flow is bottom
			{
				xd_new = yd;
				x_flow_new = y_flow;
				yd = 0.0;
				y_flow = 0.0;
			}
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 9:	//(t && !r && b && !l)
			yd_new = xd + yd;
			y_flow_new = x_flow + y_flow;
			xd_new = 0.0;
			x_flow_new = 0.0;
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 10:	//(t && r && !b && !l)
			if (xd < 0)	//if the direction of flow is left
			{
				yd_new = -xd;
				y_flow_new = x_flow;
				xd = 0.0;
				x_flow = 0.0;
			}
			if (yd < 0)	//if the diraction of flow is bottom
			{
				xd_new = -yd;
				x_flow_new = y_flow;
				yd = 0.0;
				y_flow = 0.0;
			}
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 11:	//(!t && !r && !b && l)
			xd_new = -1.0;
			x_flow_new = x_flow + y_flow;
			yd_new = 0.0;
			y_flow_new = 0.0;
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 12:	//(t && !r && !b && !l)
			xd_new = 0.0;
			x_flow_new = 0.0;
			yd_new = 1.0;
			y_flow_new = x_flow + y_flow;
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 13:	//(!t && r && !b && !l)
			xd_new = 1.0;
			x_flow_new = x_flow + y_flow;
			yd_new = 0.0;
			y_flow_new = 0.0;
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;

		case 14:	//(!t && !r && b && !l)
			xd_new = 0.0;
			x_flow_new = 0.0;
			yd_new = -1.0;
			y_flow_new = x_flow + y_flow;
			FlowToNeighbours(xd_new, yd_new, x_flow_new, y_flow_new);
			break;
	}*/

}

void Cell::UniformFlow()	//if vectors reduce itself, this function is performed
{
	double top_flow = 0.0;
	double right_flow = 0.0;
	double bottom_flow = 0.0;
	double left_flow = 0.0;

	double top_right_flow = 0.0;
	double bottom_right_flow = 0.0;
	double bottom_left_flow = 0.0;
	double top_left_flow = 0.0;

	switch (typeOfNeighbourhood)
	{
		case 0:	//(t && r && b && l)
			if (typeOfNeighbourhoodOnSlant == 0)
			{
				top_flow = input_total / 8;
				right_flow = input_total / 8;
				bottom_flow = input_total / 8;
				left_flow = input_total / 8;

				top_right_flow = input_total / 8;
				bottom_right_flow = input_total / 8;
				bottom_left_flow = input_total / 8;
				top_left_flow = input_total / 8;

				FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
				FlowToNeighboursOnSlant(top_right_flow/2, top_right_flow/2, 0, 0, 2);
				FlowToNeighboursOnSlant(0, bottom_right_flow / 2, bottom_right_flow / 2, 0, 4);
				FlowToNeighboursOnSlant(0, 0, bottom_left_flow / 2, bottom_left_flow / 2, 6);
				FlowToNeighboursOnSlant(top_left_flow / 2, 0, 0, top_left_flow / 2, 8);
			}
			else
			{
				top_flow = 0.25 * input_total;
				right_flow = 0.25 * input_total;
				bottom_flow = 0.25 * input_total;
				left_flow = 0.25 * input_total;
				FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			}
			break;

		case 1: //(!t && r && b && l)
			top_flow = 0.0;
			right_flow = input_total / 3;
			bottom_flow = input_total / 3;
			left_flow = input_total / 3;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 2: //(t && !r && b && l)
			top_flow = input_total / 3;
			right_flow = 0.0;
			bottom_flow = input_total / 3;
			left_flow = input_total / 3;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 3: //(t && r && !b && l)
			top_flow = input_total / 3;
			right_flow = input_total / 3;
			bottom_flow = 0.0;
			left_flow = input_total / 3;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 4: //(t && r && b && !l)
			top_flow = input_total / 3;
			right_flow = input_total / 3;
			bottom_flow = input_total / 3;
			left_flow = 0.0;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 5: //(!t && !r && b && l)
			top_flow = 0.0;
			right_flow = 0.0;
			bottom_flow = input_total / 2;
			left_flow = input_total / 2;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 6: //(!t && r && !b && l)
			top_flow = 0.0;
			right_flow = input_total / 2;
			bottom_flow = 0.0;
			left_flow = input_total / 2;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 7: //(!t && r && b && !l)
			top_flow = 0.0;
			right_flow = input_total / 2;
			bottom_flow = input_total / 2;
			left_flow = 0.0;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 8: //(t && !r && !b && l)
			top_flow = input_total / 2;
			right_flow = 0.0;
			bottom_flow = 0.0;
			left_flow = input_total / 2;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 9: //(t && !r && b && !l)
			top_flow = input_total / 2;
			right_flow = 0.0;
			bottom_flow = input_total / 2;
			left_flow = 0.0;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 10: //(t && r && !b && !l)
			top_flow = input_total / 2;
			right_flow = input_total / 2;
			bottom_flow = 0.0;
			left_flow = 0.0;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 11: //(!t && !r && !b && l)
			top_flow = 0.0;
			right_flow = 0.0;
			bottom_flow = 0.0;
			left_flow = input_total;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 12: //(t && !r && !b && !l)
			top_flow = input_total;
			right_flow = 0.0;
			bottom_flow = 0.0;
			left_flow = 0.0;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 13: //(!t && r && !b && !l)
			top_flow = 0.0;
			right_flow = input_total;
			bottom_flow = 0.0;
			left_flow = 0.0;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;

		case 14: //(!t && !r && b && !l)
			top_flow = 0.0;
			right_flow = 0.0;
			bottom_flow = input_total;
			left_flow = 0.0;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
			break;
	}

}

void Cell::FlowToNeighbours(double xd, double yd, double x_flow, double y_flow)
{
	if (xd > 0)		//flow to right
	{
		neighbours["Right"]->SetLeftInput(x_flow);
		output_right_next += x_flow;
	}
	else	//flow to left
	{
		neighbours["Left"]->SetRightInput(x_flow);
		output_left_next += x_flow;
	}
	if (yd > 0)	//flow to top
	{
		neighbours["Top"]->SetBottomInput(y_flow);
		output_top_next += y_flow;
	}
	else	//flow to bottom
	{
		neighbours["Bottom"]->SetTopInput(y_flow);
		output_bottom_next += y_flow;
	}
	output_total_next = output_bottom_next + output_left_next + output_right_next + output_top_next;
}

void Cell::FlowToNeighbours2(double top_flow, double right_flow, double bottom_flow, double left_flow)
{
	neighbours["Right"]->SetLeftInput(right_flow);
	neighbours["Bottom"]->SetTopInput(bottom_flow);
	neighbours["Left"]->SetRightInput(left_flow);
	neighbours["Top"]->SetBottomInput(top_flow);
	output_right_next += right_flow;
	output_bottom_next += bottom_flow;
	output_left_next += left_flow;
	output_top_next += top_flow;
	output_total_next += output_bottom_next + output_left_next + output_right_next + output_top_next;
}

void Cell::FlowToNeighboursOnSlant(double top_flow, double right_flow, double bottom_flow, double left_flow, int direction)
{
	switch (direction)
	{
		case 2:
			this->neighboursOnSlant["TopRight"]->SetLeftInput(right_flow);
			this->neighboursOnSlant["TopRight"]->SetBottomInput(top_flow);
			break;

		case 4:
			this->neighboursOnSlant["BottomRight"]->SetLeftInput(right_flow);
			this->neighboursOnSlant["BottomRight"]->SetTopInput(bottom_flow);
			break;

		case 6:
			this->neighboursOnSlant["BottomLeft"]->SetRightInput(left_flow);
			this->neighboursOnSlant["BottomLeft"]->SetTopInput(bottom_flow);
			break;

		case 8:
			this->neighboursOnSlant["TopLeft"]->SetRightInput(left_flow);
			this->neighboursOnSlant["TopLeft"]->SetBottomInput(top_flow);
			break;
	}
	output_right_next += right_flow;
	output_bottom_next += bottom_flow;
	output_left_next += left_flow;
	output_top_next += top_flow;
	output_total_next += output_bottom_next + output_left_next + output_right_next + output_top_next;
}

void Cell::SetTopInput(double input)
{
	this->input_top_next += input;
}

void Cell::SetRightInput(double input)
{
	this->input_right_next += input;
}

void Cell::SetBottomInput(double input)
{
	this->input_bottom_next += input;
}

void Cell::SetLeftInput(double input)
{
	this->input_left_next += input;
}

void Cell::Update()
{
	input_top = input_top_next;
	input_right = input_right_next;
	input_bottom = input_bottom_next;
	input_left = input_left_next;

	input_total = input_top + input_right + input_bottom + input_left;
	fluid_amount = input_total;

	output_top = output_top_next;
	output_right = output_right_next;
	output_bottom = output_bottom_next;
	output_left = output_left_next;

	output_total = output_total_next;

	input_top_next = input_right_next = input_bottom_next = input_left_next = 0.0;
	output_top_next = output_right_next = output_bottom_next = output_left_next = 0.0;
	output_total_next = 0.0;
}

int Cell::ChooseDirection(double x_direction, double y_direction)
{
	if( abs(x_direction) < 1e-5 && y_direction > 0 )	//top
		return 1;	
	if (x_direction > 0 && y_direction > 0)		//top right
		return 2;
	if (x_direction > 0 && abs(y_direction) < 1e-5)	//right
		return 3;
	if (x_direction > 0 && y_direction < 0)		//bottom right
		return 4;
	if (abs(x_direction) < 1e-5 && y_direction < 0)	//bottom
		return 5;
	if (x_direction < 0 && y_direction < 0)		//bottom left
		return 6;
	if (x_direction < 0 && abs(y_direction) < 1e-5)	//left
		return 7;
	if (x_direction < 0 && y_direction > 0)		//top left
		return 6;

	return 0;
}