// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Grid;
class Snake;

class Game
{
public:
    Game(const Settings& settings);
    TSharedPtr<Grid> getGrid() const { return m_grid; }
    TSharedPtr<Snake> getSnake() const { return m_snake; }

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
    TSharedPtr<Snake> m_snake;
};
}  // namespace SnakeGame