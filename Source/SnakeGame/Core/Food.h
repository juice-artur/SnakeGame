// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{

class SNAKEGAME_API Food
{
public:
    Food() = default;

    void setPosition(const Position& position);
    Position getPosition();

private:
    Position m_position{Position::Zero};
};
}  // namespace SnakeGame
