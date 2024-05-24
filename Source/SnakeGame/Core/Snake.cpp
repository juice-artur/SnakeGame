// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Snake.h"


using namespace SnakeGame;

Snake::Snake(const Settings::Snake& settings) 
{
    checkf(settings.defaultSize >= 4, TEXT("Snake length is too small: %i"), settings.defaultSize);


    const auto startPos = settings.startPosition;
    for (uint32 i = 0; i < settings.defaultSize; ++i)
    {
        m_links.AddTail(Position{startPos.x - i, startPos.y});
    }
}

void SnakeGame::Snake::move(const Input& input)
{
    if (!m_lastInput.opposite(input))
    {
        m_lastInput = input;
    }
    m_links.MoveTail(m_links.GetTail(), m_links.GetHead(), m_links.GetHead()->GetValue());
    m_links.GetHead()->GetValue() += Position(m_lastInput.x, m_lastInput.y);
}
