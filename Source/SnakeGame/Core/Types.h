// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"

namespace SnakeGame
{
struct Dimensions
{
    uint32 width;
    uint32 height;
};

struct Input
{
    int8 x;
    int8 y;
};

struct Position
{
    Position(uint32 inX, uint32 inY) : x(inX), y(inY){};
    uint32 x;
    uint32 y;

    FORCEINLINE Position& operator+=(const Position& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
};

enum class CellyType
{
    Empty,
    Wall,
    Snake,
    // Food
};

struct Settings
{
    Dimensions gridSize{40, 10};
    struct Snake
    {
        uint32 defaultSize{4};
        Position startPosition{0, 0};
    } snake;
    float gameSpeed{1.0f};
};

using TSnakeList = TDoubleLinkedList<Position>;
using TPositionPtr = TSnakeList::TDoubleLinkedListNode;

}  // namespace SnakeGame