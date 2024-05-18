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

private:
    const Dimensions c_dimensions;
    TArray<CellyType> m_cells;

    void initWalls();
    void printDebug();
    FORCEINLINE int32 positionToIndex(int32 x, int32 y) const;
};
}  // namespace SnakeGame