// Fill out your copyright notice in the Description page of Project Settings.

#include "SG_GameMode.h"
#include "SnakeGame/Core/Types.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "Framework/SG_Pawn.h"
#include "World/SG_WorldTypes.h"
#include "Core/Grid.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();
    SnakeGame::Settings GS;
    GS.gridSize = SnakeGame::Dimensions({GridSize.X, GridSize.Y});
    GS.snake.defaultSize = SnakeDefaultSize;
    GS.snake.startPosition = SnakeGame::Position{GridSize.X / 2 , GridSize.Y / 2};
    GS.gameSpeed = GameSpeed;

    Game = MakeUnique<SnakeGame::Game>(GS);
    check(Game.IsValid());

    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridVisual);
    GridVisual->SetModel(Game->getGrid(), CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    SnakeVisual = GetWorld()->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridOrigin);
    SnakeVisual->SetModel(Game->getSnake(), CellSize, Game->getGrid()->getDimensions());
    SnakeVisual->FinishSpawning(GridOrigin);

    auto* PC = GetWorld()->GetFirstPlayerController();
    check(PC);

    auto* Pawn = Cast<ASG_Pawn>(PC->GetPawn());
    check(Pawn);
    check(Game->getGrid().IsValid());
    Pawn->UpdateLocation(Game->getGrid()->getDimensions(), CellSize, GridOrigin);
    check(ColorsTable);

    const auto RowsCount = ColorsTable->GetRowNames().Num();
    check(RowsCount >= 1);
    ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
    FindFog();
    UpdateColors();

    SetupInput();
}


void ASG_GameMode::NextColor() 
{
    ColorTableIndex = (ColorTableIndex + 1) % ColorsTable->GetRowNames().Num();
    UpdateColors();
}

void ASG_GameMode::UpdateColors()
{
    const auto RowName = ColorsTable->GetRowNames()[ColorTableIndex];
    const auto* ColorSet = ColorsTable->FindRow<FSnakeColors>(RowName, {});
    if (ColorSet)
    {
        GridVisual->UpdateColors(*ColorSet);

        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
}

void ASG_GameMode::SetupInput() {}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value) {}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value) {}

void ASG_GameMode::FindFog()
{
    TArray<AActor*> Fogs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), Fogs);
    if (Fogs.Num() > 0)
    {
        Fog = Cast<AExponentialHeightFog>(Fogs[0]);
    }
}


void ASG_GameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Game.IsValid())
    {
        Game->update(DeltaSeconds, SnakeInput);
    }
}

ASG_GameMode::ASG_GameMode() 
{
    PrimaryActorTick.bCanEverTick = true;
}
