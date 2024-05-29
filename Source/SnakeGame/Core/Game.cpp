// Fill out your copyright notice in the Description page of Project Settings.

#include "Game.h"
#include "Grid.h"
#include "Snake.h"
#include "Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace SnakeGame;

Game::Game(const Settings& settings) : c_settings(settings)
{
    m_grid = MakeShared<Grid>(settings.gridSize);
    m_snake = MakeShared<Snake>(settings.snake);
    m_food = MakeShared<Food>();
    updateGrid();
    generateFood();
}

void Game::update(float deltaSeconds, const Input& input)
{
    if (m_gameOver || !updateTime(deltaSeconds))
    {
        return;
    }
    move(input);

    if (died())
    {
        m_gameOver = true;
        dispatchEvent(GameplayEvent::GameOver);
    }
}

void Game::move(const Input& input)
{
    m_snake->move(input);
    updateGrid();
    if (foodTaken())
    {
        ++m_score;
        m_snake->increase();
        dispatchEvent(GameplayEvent::FoodTaken);
        generateFood();
    }
}

void Game::updateGrid()
{
    m_grid->update(m_snake->getBody(), CellType::Snake);
    m_grid->printDebug();
}

bool Game::updateTime(float deltaSeconds)
{
    m_gameTime += deltaSeconds;
    m_moveSeconds += deltaSeconds;
    if (m_moveSeconds < c_settings.gameSpeed)
    {
        return false;
    }
    m_moveSeconds = 0.0f;
    return true;
}

bool SnakeGame::Game::died()
{
    return m_grid->hitTest(m_snake->getHead(), CellType::Wall) || m_grid->hitTest(m_snake->getHead(), CellType::Snake);
}

void SnakeGame::Game::generateFood()
{
    Position foodPosition;
    if (m_grid->randomEmptyPosition(foodPosition))
    {
        m_food->setPosition(foodPosition);
        m_grid->update(m_food->getPosition(), CellType::Food);
    }
    else
    {
        m_gameOver = true;
        dispatchEvent(GameplayEvent::GameCompleted);
    }
}

bool SnakeGame::Game::foodTaken() const
{
    return m_grid->hitTest(m_snake->getHead(), CellType::Food);
}

void Game::subscribeOnGameplayEvent(GameplayEventCallback callback) 
{
    m_gameplayEventCallback = callback;
}

void Game::dispatchEvent(GameplayEvent Event)
{
    if (m_gameplayEventCallback)
    {
        m_gameplayEventCallback(Event);
    }
}
