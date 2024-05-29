// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SG_HUD.h"
#include "Core/Game.h"
#include "UI/SG_GameplayWidget.h"
#include "UI/SG_GameOverWidget.h"

void ASG_HUD::BeginPlay()
{
    Super::BeginPlay();

    GameplayWidget = CreateWidget<USG_GameplayWidget>(GetWorld(), GameplayWidgetClass);
    check(GameplayWidget);

    GameplayWidget->AddToViewport();

    GameOverWidget = CreateWidget<USG_GameOverWidget>(GetWorld(), GameOverWidgetClass);
    check(GameOverWidget);

    GameOverWidget->AddToViewport();
    GameOverWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ASG_HUD::SetModel(const TSharedPtr<SnakeGame::Game>& InGame)
{
    if (!InGame)
    {
        return;
    }

    using namespace SnakeGame;

    Game = InGame;
    GameplayWidget->SetVisibility(ESlateVisibility::Visible);
    GameOverWidget->SetVisibility(ESlateVisibility::Collapsed);
    GameplayWidget->UpdateScore(InGame->getScore());

    InGame->subscribeOnGameplayEvent(
        [&](GameplayEvent Event)
        {
            switch (Event)
            {
                case GameplayEvent::FoodTaken:  //
                    GameplayWidget->UpdateScore(InGame->getScore());
                    break;

                case GameplayEvent::GameOver:  //
                    GameplayWidget->SetVisibility(ESlateVisibility::Collapsed);
                    GameOverWidget->SetVisibility(ESlateVisibility::Visible);
                    break;
            }
        });
}

void ASG_HUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Game.IsValid())
    {
        GameplayWidget->SetGameTime(Game.Pin()->getGameTime());
    }
}
