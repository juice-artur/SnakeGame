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

    FORCEINLINE bool opposite(const Input& rhs) const { return (x == -rhs.x && x != 0) || (y == -rhs.y && y != 0); }

    static Input Default;
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

    FORCEINLINE bool operator==(const Position& rhs) { return x == rhs.x && y == rhs.y; }

    static Position Zero;
};

enum class CellyType
{
    Empty,
    Wall,
    Snake,
    Food,
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

using TPositionPtr = TDoubleLinkedList<Position>::TDoubleLinkedListNode;

class TSnakeList : public TDoubleLinkedList<Position>
{
public:
    void MoveTail(TPositionPtr* Tail, TPositionPtr* Head, const Position& Pos)
    {
        // @todo: make real movement of tail node without remove/insert
        RemoveNode(Tail);
        InsertNode(Pos, Head->GetNextNode());
    }
};

}  // namespace SnakeGame