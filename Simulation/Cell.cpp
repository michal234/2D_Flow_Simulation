#include "Cell.h"

Cell::Cell()
{
	input_top = input_right = input_bottom = input_left = 0.0;
	input_top_next = input_right_next = input_bottom_next = input_left_next = 0.0;
	input_total = 0.0;

	output_top = output_right = output_bottom = output_left = 0.0;
	output_top_next = output_right_next = output_bottom_next = output_left_next = 0.0;
	output_total = output_total_next = 0.0;

	source = false;
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

void Cell::SetNeighbours(map<string, Cell*> neighbours)
{
	this->neighbours = neighbours;
}

void Cell::SetTypeOfNeighbourhood(int type)
{
	this->typeOfNeighbourhood = type;
}

void Cell::FluidFlow()
{
	//calculate direction of output
	double xd = input_left - input_right;
	double yd = input_bottom - input_top;
	

	if (abs(xd) < 1e-5 && abs(yd) < 1e-5)
	{
		UniformFlow();
		return;
	}

	double x_flow = input_total * abs(xd) / (abs(xd) + abs(yd));
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
	}

	
}

void Cell::UniformFlow()	//if vectors reduce itself, this function is performed
{
	double top_flow = 0.0;
	double right_flow = 0.0;
	double bottom_flow = 0.0;
	double left_flow = 0.0;

	switch (typeOfNeighbourhood)
	{
		case 0:	//(t && r && b && l)
			top_flow = 0.25 * input_total;
			right_flow = 0.25 * input_total;
			bottom_flow = 0.25 * input_total;
			left_flow = 0.25 * input_total;
			FlowToNeighbours2(top_flow, right_flow, bottom_flow, left_flow);
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
	output_total_next = output_bottom_next + output_left_next + output_right_next + output_top_next;
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