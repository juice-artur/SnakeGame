// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGame/Core/Game.h"
#include "Engine/DataTable.h"
#include "InputActionValue.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;
class ASG_Snake;
class ASG_Food;
class ASG_HUD;
class AExponentialHeightFog;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    ASG_GameMode();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    bool bOverrideUserSettings{false};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100", EditCondition = "bOverrideUserSettings", EditConditionHides),
        Category = "Settings")
    FUintPoint GridDims{10, 10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100", EditCondition = "bOverrideUserSettings", EditConditionHides),
        Category = "Settings")
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "4", ClampMax = "10", EditCondition = "bOverrideUserSettings", EditConditionHides),
        Category = "Settings")
    uint32 SnakeDefaultSize{5};

    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.01", ClampMax = "10", EditCondition = "bOverrideUserSettings", EditConditionHides),
        Category = "Settings")
    float GameSpeed{1.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TSubclassOf<ASG_Grid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TSubclassOf<ASG_Snake> SnakeVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TSubclassOf<ASG_Food> FoodVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    UDataTable* ColorsTable;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> MoveForwardInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> MoveRightInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> ResetGameInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputMappingContext> InputMapping;

private:
    uint32 ColorTableIndex{0};
    TSharedPtr<SnakeGame::Game> Game;
    SnakeGame::Input SnakeInput{SnakeGame::Input::Default};

    UPROPERTY()
    TObjectPtr<ASG_Grid> GridVisual;

    UPROPERTY()
    TObjectPtr<ASG_Snake> SnakeVisual;

    UPROPERTY()
    TObjectPtr<ASG_Food> FoodVisual;

    UPROPERTY()
    TObjectPtr<AExponentialHeightFog> Fog;

    UPROPERTY()
    TObjectPtr<ASG_HUD> HUD;

    UFUNCTION(Exec, Category = "Console command") 
    void NextColor();

    void FindFog();
    void UpdateColors();

    void SetupInput();
    void OnMoveForward(const FInputActionValue& Value);
    void OnMoveRight(const FInputActionValue& Value);
    void OnGameReset(const FInputActionValue& Value);

    SnakeGame::Settings MakeSettings() const;

    void SubscribeOnGameEvents();
};
