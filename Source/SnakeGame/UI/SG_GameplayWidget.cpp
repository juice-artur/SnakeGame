// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SG_GameplayWidget.h"
#include "Components/TextBlock.h"
#include <World/SG_WorldUtils.h>

void USG_GameplayWidget::SetGameTime(float Seconds)
{
    if (TimeText)
    {
        TimeText->SetText(SnakeGame::WorldUtils::FormatSeconds(Seconds));
    }
}

void USG_GameplayWidget::UpdateScore(uint32 Score) 
{
    if (ScoreText)
    {
        ScoreText->SetText(SnakeGame::WorldUtils::FormatScore(Score));
    }
}
