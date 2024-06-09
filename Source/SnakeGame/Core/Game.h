// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Grid;
class Snake;
class Food;

class Game
{
public:
    Game(const Settings& settings);
    TSharedPtr<Grid> getGrid() const { return m_grid; }
    TSharedPtr<Snake> getSnake() const { return m_snake; }
    TSharedPtr<Food> getFood() const { return m_food; }
    void update(float deltaSeconds, const Input& input);

    uint32 getScore() const { return m_score; }
    float getGameTime() const { return m_gameTime; }

    void subscribeOnGameplayEvent(GameplayEventCallback callback);

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
    TSharedPtr<Snake> m_snake;
    TSharedPtr<Food> m_food;

    TArray<GameplayEventCallback> m_gameplayEventCallbacks;

    float m_moveSeconds{0.0f};
    bool m_gameOver{false};
    uint32 m_score{0};
    float m_gameTime{0.0f};

    void move(const Input& input);
    void updateGrid();
    bool updateTime(float deltaSeconds);
    bool died(const Position& prevTailPosition) const;
    void generateFood();
    bool foodTaken() const;

    FORCEINLINE void dispatchEvent(GameplayEvent Event);
};
}  // namespace SnakeGame