// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_StartGameWidget.generated.h"

class UButton;

UCLASS()
class SNAKEGAME_API USG_StartGameWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StartGameButton;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> GameLevel;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStartGame();
};
