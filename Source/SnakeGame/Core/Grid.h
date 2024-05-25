// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{

class Grid
{
public:
    Grid(const Dimensions& dim);
    Dimensions getDimensions() const { return c_dimensions; }
    void printDebug();
    void update(const TPositionPtr* links, CellType cellType);
    void update(const Position& possition, CellType cellType);
    bool hitTest(const Position& possition, CellType cellType) const;
    static Position center(uint32 width, uint32 height) { return Position(width / 2 + 1, height / 2 + 1); }

private:
    const Dimensions c_dimensions;
    TArray<CellType> m_cells;
    TMap<CellType, TArray<uint32>> m_indByType = {
        {CellType::Snake, {}},  //
        {CellType::Wall, {}},   //
        {CellType::Food, {}},   //
    };

    void initWalls();
    FORCEINLINE int32 possitionToIndex(int32 x, int32 y) const;
    FORCEINLINE int32 possitionToIndex(const Position& possition) const;
    void freeCellsByType(CellType cellType);
    void updateInternal(const Position& possition, CellType cellType);
};
}  // namespace SnakeGame