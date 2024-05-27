// Fill out your copyright notice in the Description page of Project Settings.

#include "SG_GameMode.h"
#include "SnakeGame/Core/Types.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_Food.h"
#include "Framework/SG_Pawn.h"
#include "World/SG_WorldTypes.h"
#include "Core/Grid.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnakeGameMode, All, All);

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    Game = MakeUnique<SnakeGame::Game>(MakeSettings());
    check(Game.IsValid());
    SubscribeOnGameEvents();

    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridVisual);
    GridVisual->SetModel(Game->getGrid(), CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    SnakeVisual = GetWorld()->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridOrigin);
    SnakeVisual->SetModel(Game->getSnake(), CellSize, Game->getGrid()->getDimensions());
    SnakeVisual->FinishSpawning(GridOrigin);

    FoodVisual = GetWorld()->SpawnActorDeferred<ASG_Food>(FoodVisualClass, GridOrigin);
    FoodVisual->SetModel(Game->getFood(), CellSize, Game->getGrid()->getDimensions());
    FoodVisual->FinishSpawning(GridOrigin);

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
        SnakeVisual->UpdateColor(*ColorSet);
        FoodVisual->UpdateColor(ColorSet->FoodColor);

        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
}

void ASG_GameMode::SetupInput()
{
    if (!GetWorld()) return;

    if (auto* PC = GetWorld()->GetFirstPlayerController())
    {
        if (auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }

        auto* Input = Cast<UEnhancedInputComponent>(PC->InputComponent);
        check(Input);
        Input->BindAction(MoveForwardInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveForward);
        Input->BindAction(MoveRightInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveRight);
        Input->BindAction(ResetGameInputAction, ETriggerEvent::Started, this, &ThisClass::OnGameReset);
    }
}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0) return;
    SnakeInput = SnakeGame::Input{0, static_cast<int8>(InputValue)};
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0) return;
    SnakeInput = SnakeGame::Input{static_cast<int8>(InputValue), 0};
}

void ASG_GameMode::OnGameReset(const FInputActionValue& Value)
{
    if (const bool InputValue = Value.Get<bool>())
    {
        Game.Reset(new SnakeGame::Game(MakeSettings()));
        check(Game.IsValid());
        SubscribeOnGameEvents();
        GridVisual->SetModel(Game->getGrid(), CellSize);
        SnakeVisual->SetModel(Game->getSnake(), CellSize, Game->getGrid()->getDimensions());
        FoodVisual->SetModel(Game->getFood(), CellSize, Game->getGrid()->getDimensions());
        SnakeInput = SnakeGame::Input::Default;
        NextColor();
    }
}

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

SnakeGame::Settings ASG_GameMode::MakeSettings() const
{
    SnakeGame::Settings GS;
    GS.gridSize = SnakeGame::Dimensions{GridSize.X, GridSize.Y};
    GS.gameSpeed = GameSpeed;
    GS.snake.defaultSize = SnakeDefaultSize;
    GS.snake.startPosition = SnakeGame::Grid::center(GridSize.X, GridSize.Y);
    return GS;
}

void ASG_GameMode::SubscribeOnGameEvents()
{
    using namespace SnakeGame;
    Game->subscribeOnGameplayEvent(
        [&](GameplayEvent Event)
        {
            switch (Event)
            {
                case GameplayEvent::GameOver:
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- GAME OVER --------------"));
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- SCORE: %i --------------"), Game->score());
                    SnakeVisual->Explode();
                    FoodVisual->Hide();
                    break;
                case GameplayEvent::GameCompleted:
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- GAME COMPLETED --------------"));
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- SCORE: %i --------------"), Game->score());
                    break;
                case GameplayEvent::FoodTaken:  //
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- FOOD TAKEN --------------"));
                    FoodVisual->Explode();
                    break;
            }
        });
}
