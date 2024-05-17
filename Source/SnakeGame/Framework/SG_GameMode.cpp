// Fill out your copyright notice in the Description page of Project Settings.

#include "SG_GameMode.h"
#include "SnakeGame/Core/Types.h"
#include "Word/SG_Grid.h"

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();
    const SnakeGame::Settings GS({GridSize.X, GridSize.Y});
    Game = MakeUnique<SnakeGame::Game>(GS);
    check(Game.IsValid())

    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridVisual);
    GridVisual->SetModel(Game->getGrid(), CellSize);
    GridVisual->FinishSpawning(GridOrigin);
}
