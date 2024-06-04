// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_GameOverWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SNAKEGAME_API USG_GameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetScore(uint32 Score);
    void SetResetGameKeyName(const FString& ResetGameKeyName);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BackToMenuButton;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> MenuLevel;

    virtual void NativeOnInitialized();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ResetGameText;

private:
    UFUNCTION()
    void OnBackToMenu();
};
