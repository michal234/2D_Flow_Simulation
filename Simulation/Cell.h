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

		map<string, &Cell> neighbours;	//neighbours of the cell
		int typeOfNeighbourhood;

		double fluid_amount;	//amount of fluid in the cell (= total_input)

		//amount of fluid that flows into the cell in current step
		double input_top;
		double input_right;
		double input_bottom;
		double input_left;

		//amount of fluid that flows into the cell in the next step
		double input_top_next;
		double input_right_next;
		double input_bottom_next;
		double input_left_next;

		//total amount of fluid that flows into the cell in the current step
		double input_total;

		//amount of fluid that flows from the cell to the neighbouring cells in the current step
		double output_top;
		double output_right;
		double output_bottom;
		double output_left;

		//amount of fluid that flows from the cell to the neighbouring cells in the next step
		double output_top_next;
		double output_right_next;
		double output_bottom_next;
		double output_left_next;

		//total amount of fluid that flows from the cell to the neighbours in the current step
		double output_total;
		double output_total_next;

		void UniformFlow();
		void FlowToNeighbours(double xd, double yd, double x_flow, double y_flow);
		void FlowToNeighbours2(double top_flow, double right_flow, double bottom_flow, double left_flow);

	public:
		Cell();
		Cell(bool fluid);	//for usage of setting neighbours in solver
		Cell(bool fluid, bool boundary, int x, int y);
		bool GetFluid();
		bool GetBoundary();
		int GetX();
		int GetY();

		void SetNeighbours(map<string, Cell> neighbours);
		void SetTypeOfNeighbourhood(int type);

		void FluidFlow();

		void SetTopInput(double input);
		void SetRightInput(double input);
		void SetBottomInput(double input);
		void SetLeftInput(double input);

		void Update();
};

