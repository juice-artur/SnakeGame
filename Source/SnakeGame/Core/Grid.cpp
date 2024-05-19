// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace SnakeGame;

Grid::Grid(const Dimensions& dim) : c_dimensions(Dimensions{dim.width + 2, dim.height + 2})
{
    m_cells.Init(CellyType::Empty, c_dimensions.width * c_dimensions.height);
    initWalls();
    printDebug();
}

void Grid::initWalls()
{
    for (uint32 y = 0; y < c_dimensions.height; y++)
    {
        for (uint32 x = 0; x < c_dimensions.width; x++)
        {
            if (x == 0 || x == c_dimensions.width - 1 || y == 0 || y == c_dimensions.height - 1)
            {
                m_cells[possitionToIndex(x, y)] = CellyType::Wall;
            }
        }
    }
}

void Grid::printDebug()
{
    for (uint32 y = 0; y < c_dimensions.height; y++)
    {
        FString line;
        for (uint32 x = 0; x < c_dimensions.width; x++)
        {
            TCHAR symbol;
            switch (m_cells[possitionToIndex(x, y)])
            {
                case CellyType::Empty: symbol = '0'; break;
                case CellyType::Wall: symbol = '*'; break;
                case CellyType::Snake: symbol = '@'; break;
                default: break;
            }
            line.AppendChar(symbol).AppendChar(' ');
        }
        UE_LOG(LogGrid, Display, TEXT("%s"), *line);
    }
}

void Grid::update(const TPositionPtr* links, CellyType cellType)
{
    freeCellsByType(cellType);
    auto* link = links;
    while (link)
    {
        const auto index = possitionToIndex(link->GetValue());
        m_cells[index] = cellType;
        link = link->GetNextNode();
    }
}


void Grid::freeCellsByType(CellyType cellType) 
{
    for (auto& cell : m_cells)
    {
        if (cell == cellType)
        {   
            cell = CellyType::Empty;
        }
    }
}


bool Grid::hitTest(const Position& possition, CellyType cellType) const
{
    return m_cells[possitionToIndex(possition)] == cellType;
}

int32 Grid::possitionToIndex(int32 x, int32 y) const
{
    return x + y * c_dimensions.width;
}

int32 Grid::possitionToIndex(const Position& possition) const
{
    return possitionToIndex(possition.x, possition.y);
}

