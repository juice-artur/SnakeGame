// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_HUD.generated.h"

class USG_GameplayWidget;
class USG_GameOverWidget;

namespace SnakeGame
{
class Game;
}  // namespace SnakeGame

UCLASS()
class SNAKEGAME_API ASG_HUD : public AHUD
{
    GENERATED_BODY()

public:
    void SetModel(const TSharedPtr<SnakeGame::Game>& InGame);
    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_GameplayWidget> GameplayWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_GameOverWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<USG_GameplayWidget> GameplayWidget;

    UPROPERTY()
    TObjectPtr<USG_GameOverWidget> GameOverWidget;

    TWeakPtr<SnakeGame::Game> Game;
};