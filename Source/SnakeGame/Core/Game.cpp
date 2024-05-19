// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Grid.h"
#include "Snake.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace SnakeGame;

Game::Game(const Settings& settings) : c_settings(settings)
{
    m_grid = MakeShared<Grid>(settings.gridSize);
    m_snake = MakeShared<Snake>(settings.snake);

    m_grid->update(m_snake->getBody(), CellyType::Snake);
    m_grid->printDebug();
}
