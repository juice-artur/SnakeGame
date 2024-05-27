// Fill out your copyright notice in the Description page of Project Settings.

#include "World/SG_Food.h"
#include "Components/StaticMeshComponent.h"
#include "SnakeGame/Core/Food.h"
#include "SG_WorldUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASG_Food::ASG_Food()
{
    PrimaryActorTick.bCanEverTick = true;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>("FoodMesh");
    check(FoodMesh);
    FoodMesh->SetupAttachment(Origin);
}

void ASG_Food::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (Food.IsValid())
    {
        SetActorLocation(GetFoodWorldLocation());
    }
}

void ASG_Food::SetModel(const TSharedPtr<SnakeGame::Food>& InFood, uint32 InCellSize, const SnakeGame::Dimensions& InDims)
{
    Food = InFood;
    CellSize = InCellSize;
    Dims = InDims;

    SnakeGame::WorldUtils::ScaleMesh(FoodMesh, FVector(CellSize));
    SetActorHiddenInGame(false);
}

void ASG_Food::UpdateColor(const FLinearColor& Color)
{
    FoodColor = Color;
    if (auto* FoodMaterial = FoodMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        FoodMaterial->SetVectorParameterValue("Color", Color);
    }
}

void ASG_Food::Explode()
{
    if (UNiagaraComponent* NS = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetFoodWorldLocation()))
    {
        NS->SetNiagaraVariableLinearColor("SnakeColor", FoodColor);
    }
}

void ASG_Food::Hide()
{
    SetActorHiddenInGame(true);
}

FVector ASG_Food::GetFoodWorldLocation() const
{
    if (!Food.IsValid())
    {
        return FVector::ZeroVector;
    }
    return SnakeGame::WorldUtils::LinkPositionToVector(Food.Pin()->getPosition(), CellSize, Dims);
}