#include "Solver.h"

Solver::Solver()
{
	initialized = false;
	boundaryConditions = false;
	cellGridCols = 0;
	cellGridRows = 0;
	v_start = 0.0;
}

bool Solver::GetInitialized()
{
	return initialized;
}

bool Solver::GetBoudaryConditions()
{
	return boundaryConditions;
}

void Solver::CellGridInitialization(BinaryMap bm)
{
	cout << "Trwa inicjalizacja siatki automatow...\n";

	cellGridRows = bm.GetRows();
	cellGridCols = bm.GetCols();
	for (int i = 0; i < cellGridRows; i++)
	{
		//vector<Cell> vc = vector<Cell>();
		for (int j = 0; j < cellGridCols; j++)
		{
			int element = bm.GetElement(i, j);
			bool fluid = true;
			bool boundary = false;

			if (element == 0)	//solid
				fluid = false;
			if( i==0 || i == cellGridRows-1 || j ==0 || j==cellGridCols-1 )
				boundary = true;

			Cell cell(fluid, boundary, i, j);
			//vc.push_back(&cell);

			CellGrid.push_back(cell);	
			
		}
		//CellGrid.push_back(vc);
	}

	for (int i = 0; i < cellGridRows; i++)
	{
		for (int j = 0; j < cellGridCols; j++)
		{
			if (CellGrid[i * cellGridCols + j].GetFluid())
			//if( CellGrid[i][j].GetFluid() )
			{
				FluidCells.push_back(&CellGrid[i*cellGridCols+j]);
				//FluidCells.push_back(&CellGrid[i][j]);
			}
		}
	}

	initialized = true;

	cout << "Inicjalizacja zakonczona\n";
}

void Solver::SetBoundaryConditions(double v)
{
	cout << "Trwa ustalanie warunkow brzegowych...\n";

	for (int i = 0; i < FluidCells.size(); i++)
	{
		if (FluidCells[i]->GetY() == 0 && FluidCells[i]->GetFluid())
		{
			FluidCells[i]->SetLeftInput(v);
			FluidCells[i]->SetSource();
		}
	}
	/*for (int i = 0; i < cellGridRows; i++)
	{
		if (CellGrid[i][0]->GetFluid())
		{
			CellGrid[i][0]->SetLeftInput(v);
			CellGrid[i][0]->SetSource();
		}
	}*/

	boundaryConditions = true;
	v_start = v;

	cout << "Ustalanie warunkow brzegowych zakonczone\n";
}

void Solver::Simulate()
{
	SetNeighbours();
	UpdateGrid();

	int unbalancedCells = 0;

	do
	{
		unbalancedCells = 0;
		for (int i = 0; i < FluidCells.size(); i++)
		{
			FluidCells[i]->FluidFlow();
			if( !FluidCells[i]->GetBalance() )
			{
				unbalancedCells++;
			}
			/*if (slant)
			{
				double top = FluidCells[i]->GetInputTop();
				double right = FluidCells[i]->GetInputRight();
				double bottom = FluidCells[i]->GetInputBottom();
				double left = FluidCells[i]->GetInputLeft();

				double total = top + right + bottom + left;

				int x = FluidCells[i]->GetX();
				int y = FluidCells[i]->GetY();

				double x_direction = left - right;
				double y_direction = bottom - top;

				double alpha = atan(y_direction / x_direction) * 180 / PI;

				if (alpha > 0 && x_direction > 0 && y_direction > 0)	//move to top right
				{
					if (!CellGrid[(x - 1) * cellGridCols + y + 1].GetFluid())		//top right is solid
					{
						CellGrid[(x - 1) * cellGridCols + y].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)));
						CellGrid[x * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)));
					}
					else
					{
						double angle_direction = alpha / 90;
						if (angle_direction <= 0.5)		//move to top right and right
						{
							CellGrid[(x - 1) * cellGridCols + y + 1].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[(x - 1) * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[x * cellGridCols + y + 1].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
							CellGrid[x * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
						}
						else	//move to top and top right
						{
							CellGrid[(x - 1) * cellGridCols + y].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x - 1) * cellGridCols + y].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x - 1) * cellGridCols + y + 1].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
							CellGrid[(x - 1) * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
						}
					}
				}
				else if (alpha < 0 && x_direction < 0 && y_direction > 0)	//move to top left
				{
					if (!CellGrid[(x - 1) * cellGridCols + y - 1].GetFluid())		//top left is solid
					{
						CellGrid[(x - 1) * cellGridCols + y].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)));
						CellGrid[x * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)));
					}
					else
					{
						double angle_direction = -alpha / 90;
						if (angle_direction <= 0.5)		//move to top left and left
						{
							CellGrid[(x - 1) * cellGridCols + y - 1].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[(x - 1) * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[x * cellGridCols + y - 1].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
							CellGrid[x * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
						}
						else	//move to top and top left
						{
							CellGrid[(x - 1) * cellGridCols + y].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x - 1) * cellGridCols + y].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x - 1) * cellGridCols + y - 1].SetBottomInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
							CellGrid[(x - 1) * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
						}
					}
				}
				else if (alpha > 0 && x_direction < 0 && y_direction < 0)	//move to bottom left
				{
					if (!CellGrid[(x + 1) * cellGridCols + y - 1].GetFluid())		//bottom left is solid
					{
						CellGrid[(x + 1) * cellGridCols + y].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)));
						CellGrid[x * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)));
					}
					else
					{
						double angle_direction = alpha / 90;
						if (angle_direction <= 0.5)		//move to bottom left and left
						{
							CellGrid[(x + 1) * cellGridCols + y - 1].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[(x + 1) * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[x * cellGridCols + y - 1].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
							CellGrid[x * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
						}
						else	//move to bottom and bottom left
						{
							CellGrid[(x + 1) * cellGridCols + y].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x + 1) * cellGridCols + y].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x + 1) * cellGridCols + y - 1].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
							CellGrid[(x + 1) * cellGridCols + y - 1].SetRightInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
						}
					}
				}
				else if (alpha < 0 && x_direction > 0 && y_direction < 0)	//move to bottom right
				{
					if (!CellGrid[(x + 1) * cellGridCols + y + 1].GetFluid())		//bottom right is solid
					{
						CellGrid[(x + 1) * cellGridCols + y].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)));
						CellGrid[x * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)));
					}
					else
					{
						double angle_direction = -alpha / 90;
						if (angle_direction <= 0.5)		//move to bottom right and right
						{
							CellGrid[(x + 1) * cellGridCols + y + 1].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[(x + 1) * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * angle_direction / 0.5);
							CellGrid[x * cellGridCols + y + 1].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
							CellGrid[x * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - angle_direction) / 0.5);
						}
						else	//move to bottom and bottom right
						{
							CellGrid[(x + 1) * cellGridCols + y].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x + 1) * cellGridCols + y].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (angle_direction - 0.5) / 0.5);
							CellGrid[(x + 1) * cellGridCols + y + 1].SetTopInput(total * abs(y_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
							CellGrid[(x + 1) * cellGridCols + y + 1].SetLeftInput(total * abs(x_direction) / (abs(x_direction) + abs(y_direction)) * (1 - (angle_direction - 0.5)) / 0.5);
						}
					}
				}
				FluidCells[i]->SetOutputTotal(total);
			}*/
		}
		UpdateGrid();

		ShowStep();
		cout << endl<< "Liczba komorek niezbilansowanych: " << unbalancedCells << endl;
		cout << endl << endl;
	}while(unbalancedCells != 0);
}

void Solver::SetNeighbours()
{
	cout << "Trwa przydzielanie sasiadow dla kazdej komorki...\n";

	for (int i = 0; i < FluidCells.size(); i++)
	{
		Cell* cell = FluidCells[i];
		int x = cell->GetX();
		int y = cell->GetY();
		//map<string, Cell*> neighbours = map<string, Cell*>();

		Cell outer = Cell(true);	//default fluid cell for cells on the boundary

		Cell* top;
		Cell* right;
		Cell* bottom;
		Cell* left;

		Cell* topRight;
		Cell* bottomRight;
		Cell* bottomLeft;
		Cell* topLeft;

		int type = 0;
		int type_slant = 0;


		if (!cell->GetBoundary())	//if the cell lays in the interior
		{
			/*Cell* top = &CellGrid[x-1][y];
			Cell* right = &CellGrid[x][y+1];
			Cell* bottom = &CellGrid[x+1][y];
			Cell* left = &CellGrid[x][y-1];*/

			top = &CellGrid[(x-1)*cellGridCols + y];
			right = &CellGrid[x*cellGridCols + y + 1];
			bottom = &CellGrid[(x+1)*cellGridCols + y];
			left = &CellGrid[x*cellGridCols + y - 1];

			topRight = &CellGrid[(x-1)*cellGridCols + y + 1];
			bottomRight = &CellGrid[(x+1)*cellGridCols + y + 1];
			bottomLeft = &CellGrid[(x+1)*cellGridCols + y - 1];
			topLeft = &CellGrid[(x-1)*cellGridCols + y - 1];

			type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
			type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);

		}
		else	//if the cell lays on the boundary
		{
			if (x == 0)		//top boundary
			{
				if (y == 0)	//top left corner
				{
					/*Cell* top = &outer;
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &CellGrid[x + 1][y];
					Cell* left = &outer;*/

					top = &outer;
					right = &CellGrid[x*cellGridCols + y + 1];
					bottom = &CellGrid[(x+1)*cellGridCols + y];
					left = &outer;

					topRight = &outer;
					bottomRight = &CellGrid[(x + 1) * cellGridCols + y + 1];
					bottomLeft = &outer;
					topLeft = &outer;

					type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
					type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);

				}
				else if (y == cellGridCols - 1)	//top right corner
				{
					/*Cell* top = &outer;
					Cell* right = &outer;
					Cell* bottom = &CellGrid[x + 1][y];
					Cell* left = &CellGrid[x][y - 1];*/

					top = &outer;
					right = &outer;
					bottom = &CellGrid[(x+1)*cellGridCols + y];
					left = &CellGrid[x*cellGridCols + y - 1];

					topRight = &outer;
					bottomRight = &outer;
					bottomLeft = &CellGrid[(x + 1) * cellGridCols + y - 1];
					topLeft = &outer;

					type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
					type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);
				}
				else	//the rest of the top boundary
				{
					/*Cell* top = &outer;
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &CellGrid[x + 1][y];
					Cell* left = &CellGrid[x][y - 1];*/

					top = &outer;
					right = &CellGrid[x*cellGridCols + y + 1];
					bottom = &CellGrid[(x+1)*cellGridCols + y];
					left = &CellGrid[x * cellGridCols + y - 1];

					topRight = &outer;
					bottomRight = &CellGrid[(x + 1) * cellGridCols + y + 1];
					bottomLeft = &CellGrid[(x + 1) * cellGridCols + y - 1];
					topLeft = &outer;

					type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
					type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);
				}
			}
			else if (x == cellGridRows - 1)	//bottom boundary
			{
				if (y == 0)	//bottom left corner
				{
					/*Cell* top = &CellGrid[x - 1][y];
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &outer;
					Cell* left = &outer;*/

					top = &CellGrid[(x-1)*cellGridCols + y];
					right = &CellGrid[x * cellGridCols + y + 1];
					bottom = &outer;
					left = &outer;

					topRight = &CellGrid[(x - 1) * cellGridCols + y + 1];
					bottomRight = &outer;
					bottomLeft = &outer;
					topLeft = &outer;

					type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
					type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);
				}
				else if (y == cellGridCols - 1)	//bottom right corner
				{
					/*Cell* top = &CellGrid[x - 1][y];
					Cell* right = &outer;
					Cell* bottom = &outer;
					Cell* left = &CellGrid[x][y - 1];*/

					top = &CellGrid[(x-1)*cellGridCols + y];
					right = &outer;
					bottom = &outer;
					left = &CellGrid[x*cellGridCols + y - 1];

					topRight = &outer;
					bottomRight = &outer;
					bottomLeft = &outer;
					topLeft = &CellGrid[(x - 1) * cellGridCols + y - 1];

					type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
					type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);
				}
				else	//the rest of the bottom boundary
				{
					/*Cell* top = &CellGrid[x - 1][y];
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &outer;
					Cell* left = &CellGrid[x][y - 1];*/

					top = &CellGrid[(x-1)*cellGridCols + y];
					right = &CellGrid[x*cellGridCols + y + 1];
					bottom = &outer;
					left = &CellGrid[x*cellGridCols + y - 1];

					topRight = &CellGrid[(x - 1) * cellGridCols + y + 1];
					bottomRight = &outer;
					bottomLeft = &outer;
					topLeft = &CellGrid[(x - 1) * cellGridCols + y - 1];

					type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
					type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);
				}
			}
			else if (y == 0)	//left boundary (without corners)
			{
				/*Cell* top = &CellGrid[x - 1][y];
				Cell* right = &CellGrid[x][y + 1];
				Cell* bottom = &CellGrid[x + 1][y];
				Cell* left = &outer;*/

				top = &CellGrid[(x-1)*cellGridCols + y];
				right = &CellGrid[x*cellGridCols + y + 1];
				bottom = &CellGrid[(x+1)*cellGridCols + y];
				left = &outer;

				topRight = &CellGrid[(x - 1) * cellGridCols + y + 1];
				bottomRight = &CellGrid[(x + 1) * cellGridCols + y + 1];
				bottomLeft = &outer;
				topLeft = &outer;

				type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
				type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);
			}
			else	//right boundary (without corners)
			{
				/*Cell* top = &CellGrid[x - 1][y];
				Cell* right = &outer;
				Cell* bottom = &CellGrid[x + 1][y];
				Cell* left = &CellGrid[x][y - 1];*/

				top = &CellGrid[(x-1)*cellGridCols + y];
				right = &outer;
				bottom = &CellGrid[(x+1)*cellGridCols + y];
				left = &CellGrid[x*cellGridCols + y - 1];
				
				topRight = &outer;
				bottomRight = &outer;
				bottomLeft = &CellGrid[(x + 1) * cellGridCols + y - 1];
				topLeft = &CellGrid[(x - 1) * cellGridCols + y - 1];

				type = TypeOfNeighbourhood(*top, *right, *bottom, *left);
				type_slant = TypeOfNeighbourhood(*topRight, *bottomRight, *bottomLeft, *topLeft);
			}
		}

		cell->SetNeighbours(top, right, bottom, left);
		cell->SetTypeOfNeighbourhood(type);
		cell->SetNeighboursOnSlant(topRight, bottomRight, bottomLeft, topLeft);
		cell->SetTypeOfNeighbourhoodOnSlant(type_slant);
	}

	cout << "Przydzielanie sasiadow zakonczone\n";
}

int Solver::TypeOfNeighbourhood(Cell top, Cell right, Cell bottom, Cell left)	//(*topRight, *bottomRight, *bottomLeft, *topLeft) for slant
{
	bool t = top.GetFluid();
	bool r = right.GetFluid();
	bool b = bottom.GetFluid();
	bool l = left.GetFluid();

	if( t && r && b && l )
		return 0;
	if (!t && r && b && l)
		return 1;
	if (t && !r && b && l)
		return 2;
	if (t && r && !b && l)
		return 3;
	if (t && r && b && !l)
		return 4;
	if (!t && !r && b && l)
		return 5;
	if (!t && r && !b && l)
		return 6;
	if (!t && r && b && !l)
		return 7;
	if (t && !r && !b && l)
		return 8;
	if (t && !r && b && !l)
		return 9;
	if (t && r && !b && !l)
		return 10;
	if (!t && !r && !b && l)
		return 11;
	if (t && !r && !b && !l)
		return 12;
	if (!t && r && !b && !l)
		return 13;
	if (!t && !r && b && !l)
		return 14;
	
	return 15;
}

void Solver::UpdateGrid()
{
	//cout << "Trwa aktualizacja siatki...\n";

	for (int i = 0; i < FluidCells.size(); i++)
	{
		FluidCells[i]->Update();
		if (FluidCells[i]->GetSource())
			FluidCells[i]->SetLeftInput(v_start);
	}

	//cout << "Aktualizacja siatki zakonczona\n";
}

void Solver::ShowStep()
{
	for (int i = 0; i < cellGridRows; i++)
	{
		for (int j = 0; j < cellGridCols; j++)
		{
			if( CellGrid[i*cellGridCols+j].GetFluid() )
			//if( CellGrid[i][j].GetFluid() )
				//cout << CellGrid[i][j].GetFluidAmount() << " ";
				cout << CellGrid[i*cellGridCols+j].GetFluidAmount() << " ";
				//printf("%.1f ", CellGrid[i * cellGridCols + j].GetFluidAmount() );
			else
				cout << "   ";
		}
		cout << endl;
	}
}