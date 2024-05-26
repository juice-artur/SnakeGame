// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Snake.h"
#include "World/SG_WorldTypes.h"
#include "SG_Snake.generated.h"

class ASG_SnakeLink;

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
    GENERATED_BODY()

public:
    ASG_Snake();

    void SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::Dimensions& InDims);
    void UpdateColor(const FSnakeColors& Colors);
    virtual void Tick(float DeltaTime) override;
    void Explode();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ASG_SnakeLink> SnakeHeadClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<ASG_SnakeLink> SnakeLinkClass;

private:
    TWeakPtr<SnakeGame::Snake> Snake;
    uint32 CellSize;
    SnakeGame::Dimensions Dims;
    FLinearColor SnakeLinkColor;

    UPROPERTY()
    TArray<ASG_SnakeLink*> SnakeLinks;
};
