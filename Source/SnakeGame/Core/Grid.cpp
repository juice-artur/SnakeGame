// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace SnakeGame;

Grid::Grid(const Dimensions& dim) : c_dimensions(Dimensions{dim.width + 2, dim.height + 2})
{
    m_cells.Init(CellType::Empty, c_dimensions.width * c_dimensions.height);
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
                const auto index = possitionToIndex(x, y);
                m_cells[index] = CellType::Wall;
                m_indByType[CellType::Wall].Add(index);
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
                case CellType::Empty: symbol = '0'; break;
                case CellType::Wall: symbol = '*'; break;
                case CellType::Snake: symbol = '@'; break;
                case CellType::Food: symbol = 'F'; break;
                default: break;
            }
            line.AppendChar(symbol).AppendChar(' ');
        }
        UE_LOG(LogGrid, Display, TEXT("%s"), *line);
    }
}

void Grid::update(const TPositionPtr* links, CellType cellType)
{
    freeCellsByType(cellType);
    auto* link = links;
    while (link)
    {
        updateInternal(link->GetValue(), cellType);
        link = link->GetNextNode();
    }
}

void Grid::update(const Position& possition, CellType cellType)
{
    freeCellsByType(cellType);
    updateInternal(possition, cellType);
}

void Grid::freeCellsByType(CellType cellType)
{
    for (int32 i = 0; i < m_indByType[cellType].Num(); ++i)
    {
        const uint32 ind = m_indByType[cellType][i];
        m_cells[ind] = CellType::Empty;
    }
    m_indByType[cellType].Empty();
}

void Grid::updateInternal(const Position& possition, CellType cellType)
{
    const auto index = possitionToIndex(possition);
    m_cells[index] = cellType;
    m_indByType[cellType].Add(index);
}

bool Grid::hitTest(const Position& possition, CellType cellType) const
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
