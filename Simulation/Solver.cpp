#include "Solver.h"

Solver::Solver()
{
	initialized = false;
	boundaryConditions = false;
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
		vector<Cell> vc;
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
			vc.push_back(cell);

			if( fluid )
				FluidCells.push_back(cell);
			
		}
		CellGrid.push_back(vc);
	}

	initialized = true;

	cout << "Inicjalizacja zakonczona\n";
}

void Solver::SetBoundaryConditions(double v)
{
	cout << "Trwa ustalanie warunkow brzegowych...\n";

	for (int i = 0; i < FluidCells.size(); i++)
	{
		if (FluidCells[i].GetY() == 0 && FluidCells[i].GetFluid())
		{
			FluidCells[i].SetLeftInput(v);
			FluidCells[i].SetSource();
		}
	}

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
			if( !FluidCells[i].GetBalance() )
			{
				FluidCells[i].FluidFlow();
				unbalancedCells++;
			}
		}
		UpdateGrid();
		cout << unbalancedCells << endl;
	}while(unbalancedCells != 0);
}

void Solver::SetNeighbours()
{
	cout << "Trwa przydzielanie sasiadow dla kazdej komorki...\n";

	for (int i = 0; i < FluidCells.size(); i++)
	{
		Cell* cell = &FluidCells[i];
		int x = cell->GetX();
		int y = cell->GetY();
		map<string, Cell*> neighbours = map<string, Cell*>();

		Cell outer = Cell(true);	//default fluid cell for cells on the boundary


		if (!cell->GetBoundary())	//if the cell lays in the interior
		{
			Cell* top = &CellGrid[x-1][y];
			Cell* right = &CellGrid[x][y+1];
			Cell* bottom = &CellGrid[x+1][y];
			Cell* left = &CellGrid[x][y-1];

			int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

			neighbours.insert({"Top", top});
			neighbours.insert({ "Right", right });
			neighbours.insert({ "Bottom", bottom });
			neighbours.insert({ "Left", left });

			cell->SetNeighbours(neighbours);
			cell->SetTypeOfNeighbourhood(type);
		}
		else	//if the cell lays on the boundary
		{
			if (x == 0)		//top boundary
			{
				if (y == 0)	//top left corner
				{
					Cell* top = &outer;
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &CellGrid[x + 1][y];
					Cell* left = &outer;

					int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

					neighbours.insert({ "Top", top });
					neighbours.insert({ "Right", right });
					neighbours.insert({ "Bottom", bottom });
					neighbours.insert({ "Left", left });

					cell->SetNeighbours(neighbours);
					cell->SetTypeOfNeighbourhood(type);
				}
				else if (y == cellGridCols - 1)	//top right corner
				{
					Cell* top = &outer;
					Cell* right = &outer;
					Cell* bottom = &CellGrid[x + 1][y];
					Cell* left = &CellGrid[x][y - 1];

					int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

					neighbours.insert({ "Top", top });
					neighbours.insert({ "Right", right });
					neighbours.insert({ "Bottom", bottom });
					neighbours.insert({ "Left", left });

					cell->SetNeighbours(neighbours);
					cell->SetTypeOfNeighbourhood(type);
				}
				else	//the rest of the top boundary
				{
					Cell* top = &outer;
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &CellGrid[x + 1][y];
					Cell* left = &CellGrid[x][y - 1];

					int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

					neighbours.insert({ "Top", top });
					neighbours.insert({ "Right", right });
					neighbours.insert({ "Bottom", bottom });
					neighbours.insert({ "Left", left });

					cell->SetNeighbours(neighbours);
					cell->SetTypeOfNeighbourhood(type);
				}
			}
			else if (x == cellGridRows - 1)	//bottom boundary
			{
				if (y == 0)	//bottom left corner
				{
					Cell* top = &CellGrid[x - 1][y];
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &outer;
					Cell* left = &outer;

					int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

					neighbours.insert({ "Top", top });
					neighbours.insert({ "Right", right });
					neighbours.insert({ "Bottom", bottom });
					neighbours.insert({ "Left", left });

					cell->SetNeighbours(neighbours);
					cell->SetTypeOfNeighbourhood(type);
				}
				else if (y == cellGridCols - 1)	//bottom right corner
				{
					Cell* top = &CellGrid[x - 1][y];
					Cell* right = &outer;
					Cell* bottom = &outer;
					Cell* left = &CellGrid[x][y - 1];

					int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

					neighbours.insert({ "Top", top });
					neighbours.insert({ "Right", right });
					neighbours.insert({ "Bottom", bottom });
					neighbours.insert({ "Left", left });

					cell->SetNeighbours(neighbours);
					cell->SetTypeOfNeighbourhood(type);
				}
				else	//the rest of the bottom boundary
				{
					Cell* top = &CellGrid[x - 1][y];
					Cell* right = &CellGrid[x][y + 1];
					Cell* bottom = &outer;
					Cell* left = &CellGrid[x][y - 1];

					int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

					neighbours.insert({ "Top", top });
					neighbours.insert({ "Right", right });
					neighbours.insert({ "Bottom", bottom });
					neighbours.insert({ "Left", left });

					cell->SetNeighbours(neighbours);
					cell->SetTypeOfNeighbourhood(type);
				}
			}
			else if (y == 0)	//left boundary (without corners)
			{
				Cell* top = &CellGrid[x - 1][y];
				Cell* right = &CellGrid[x][y + 1];
				Cell* bottom = &CellGrid[x + 1][y];
				Cell* left = &outer;

				int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

				neighbours.insert({ "Top", top });
				neighbours.insert({ "Right", right });
				neighbours.insert({ "Bottom", bottom });
				neighbours.insert({ "Left", left });

				cell->SetNeighbours(neighbours);
				cell->SetTypeOfNeighbourhood(type);
			}
			else	//right boundary (without corners)
			{
				Cell* top = &CellGrid[x - 1][y];
				Cell* right = &outer;
				Cell* bottom = &CellGrid[x + 1][y];
				Cell* left = &CellGrid[x][y - 1];

				int type = TypeOfNeighbourhood(*top, *right, *bottom, *left);

				neighbours.insert({ "Top", top });
				neighbours.insert({ "Right", right });
				neighbours.insert({ "Bottom", bottom });
				neighbours.insert({ "Left", left });

				cell->SetNeighbours(neighbours);
				cell->SetTypeOfNeighbourhood(type);
			}
		}

		//neighbours.clear();		//clear the map of neighbours in order to create place for the noighbours of the next cell
	}

	cout << "Przydzielanie sasiadow zakonczone\n";
}

int Solver::TypeOfNeighbourhood(Cell top, Cell right, Cell bottom, Cell left)
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
		FluidCells[i].Update();
		if (FluidCells[i].GetSource())
			FluidCells[i].SetLeftInput(v_start);
	}

	//cout << "Aktualizacja siatki zakonczona\n";
}