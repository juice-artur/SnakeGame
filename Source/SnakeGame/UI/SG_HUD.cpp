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
    GameWidgets.Add(EUIGameState::GameInProgress, GameplayWidget);

    GameOverWidget = CreateWidget<USG_GameOverWidget>(GetWorld(), GameOverWidgetClass);
    check(GameOverWidget);
    GameWidgets.Add(EUIGameState::GameOver, GameOverWidget);

    for (auto& [UIState, GameWidget] : GameWidgets)
    {
        if (GameWidget)
        {
            GameWidget->AddToViewport();
            GameWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void ASG_HUD::SetModel(const TSharedPtr<SnakeGame::Game>& InGame)
{
    if (!InGame)
    {
        return;
    }

    using namespace SnakeGame;

    Game = InGame;
    SetUIMatchState(EUIGameState::GameInProgress);
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
                    SetUIMatchState(EUIGameState::GameOver);
                    break;
            }
        });
}

void ASG_HUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Game.IsValid() && GameState == EUIGameState::GameInProgress)
    {
        GameplayWidget->SetGameTime(Game.Pin()->getGameTime());
    }
}

void ASG_HUD::SetUIMatchState(EUIGameState InGameState)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (GameWidgets.Contains(InGameState))
    {
        CurrentWidget = GameWidgets[InGameState];
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }

    GameState = InGameState;
}