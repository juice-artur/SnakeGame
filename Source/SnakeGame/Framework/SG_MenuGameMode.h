// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SG_MenuGameMode.generated.h"

UCLASS()
class SNAKEGAME_API ASG_MenuGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    void StartPlay();
};
