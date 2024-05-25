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
    void update(const TPositionPtr* links, CellyType cellType);
    bool hitTest(const Position& possition, CellyType cellType) const;
    static Position center(uint32 width, uint32 height) { return Position(width / 2 + 1, height / 2 + 1); }

private:
    const Dimensions c_dimensions;
    TArray<CellyType> m_cells;

    void initWalls();
    FORCEINLINE int32 possitionToIndex(int32 x, int32 y) const;
    FORCEINLINE int32 possitionToIndex(const Position& possition) const;
    void freeCellsByType(CellyType cellType);
};
}  // namespace SnakeGame