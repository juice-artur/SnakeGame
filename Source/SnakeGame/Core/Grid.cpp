// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace SnakeGame;

Grid::Grid(const Dimensions& dim) : c_dimensions(Dimensions{ dim.width + 2, dim.height + 2 })
{
	m_cells.Init(CellyType::Empty, c_dimensions.width * c_dimensions.height);
	initWalls();
	printDebug();
}


void Grid::initWalls()
{
	for (int32 y = 0; y < c_dimensions.height; y++)
	{
		for (int32 x = 0; y < c_dimensions.width; x++)
		{
			if (x == 0 || x == c_dimensions.width - 1 || y == 0 || y == c_dimensions.height - 1)
			{
				m_cells[positionToIndex(x, y)] = CellyType::Wall;
			}
		}
	}
}

void Grid::printDebug()
{
	for (int32 y = 0; y < c_dimensions.height; y++)
	{
		FString line;
		for (int32 x = 0; y < c_dimensions.width; x++)
		{
			TCHAR symbol;
			switch (m_cells[positionToIndex(x, y)]) {
				case CellyType::Empty: symbol = '0'; break;
				case CellyType::Wall: symbol = '*'; break;
				default: break;
			}
			line.AppendChar(symbol).AppendChar(' ');
		}
		UE_LOG(LogGrid, Display, TEXT("%s"), *line);
	}
}

int32 Grid::positionToIndex(int32 x, int32 y) const
{
	return x + y * c_dimensions.width;
}
