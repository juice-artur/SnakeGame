// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Types.h"
#include "World/SG_WorldTypes.h"
#include "SG_Food.generated.h"

namespace SnakeGame
{
class Food;
}

class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class SNAKEGAME_API ASG_Food : public AActor
{
    GENERATED_BODY()

public:
    ASG_Food();
    virtual void Tick(float DeltaTime) override;

    void SetModel(const TSharedPtr<SnakeGame::Food>& InFood, uint32 InCellSize, const SnakeGame::Dimensions& InDims);

    void UpdateColor(const FLinearColor& Color);
    void Explode();

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* FoodMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;

private:
    TWeakPtr<SnakeGame::Food> Food;
    uint32 CellSize;
    SnakeGame::Dimensions Dims;
    FLinearColor FoodColor;

    FVector GetFoodWorldLocation() const;
};
