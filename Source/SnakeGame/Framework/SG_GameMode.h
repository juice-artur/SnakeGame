// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGame/Core/Game.h"
#include "Engine/DataTable.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;
class AExponentialHeightFog;

UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    ASG_GameMode();

protected:
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"), Category = "Settings")
    FUintPoint GridSize{10, 10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"), Category = "Settings")
    int32 CellSize{10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "4", ClampMax = "10"), Category = "Settings")
    int32 SnakeDefaultSize{5};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.01", ClampMax = "10"), Category = "Settings")
    float GameSpeed{1.0};

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ASG_Grid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    UDataTable* ColorsTable;

private:
    uint32 ColorTableIndex{0};
    TUniquePtr<SnakeGame::Game> Game;
    SnakeGame::Input SnakeInput{1, 0};

    UPROPERTY()
    ASG_Grid* GridVisual;

    UPROPERTY()
    AExponentialHeightFog* Fog;

    UFUNCTION(Exec, Category = "Console command")
    void NextColor();

    void FindFog();
    void UpdateColors();
};
