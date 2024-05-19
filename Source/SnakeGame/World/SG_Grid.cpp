// Fill out your copyright notice in the Description page of Project Settings.

#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGame, All, All);

ASG_Grid::ASG_Grid()
{
    PrimaryActorTick.bCanEverTick = false;

    
    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    GridMesh = CreateDefaultSubobject<UStaticMeshComponent>("GridMesh");
    check(GridMesh);
    GridMesh->SetupAttachment(Origin);
}

void ASG_Grid::BeginPlay()
{
    Super::BeginPlay();
}

void ASG_Grid::SetModel(const TSharedPtr<SnakeGame::Grid>& Grid, int32 InCellSize)
{
    if (!Grid.IsValid())
    {
        UE_LOG(LogWorldGame, Fatal, TEXT("Grid is null, game aborted"));
    }
    GridDim = Grid.Get()->getDimensions();
    CellSize = InCellSize;
    WorldWidth = GridDim.width * CellSize;
    WorldHeight = GridDim.height * CellSize;

    check(GridMesh->GetStaticMesh());

    const FBox Box = GridMesh->GetStaticMesh()->GetBoundingBox();
    const auto Size = Box.GetSize();

    GridMesh->SetRelativeScale3D(FVector(WorldHeight / Size.X, WorldWidth/ Size.Y, 1.0f));
    GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z));

    GridMaterial = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
    if (GridMaterial)
    {
        GridMaterial->SetVectorParameterValue("Division", FVector(GridDim.height, GridDim.width, 0.0));
    }
}

void ASG_Grid::UpdateColors(const FSnakeColors& Colors) 
{
    if (GridMaterial)
    {
        GridMaterial->SetVectorParameterValue("BackgroundColor", Colors.GridBackgroundColor);
        GridMaterial->SetVectorParameterValue("LineColor", Colors.GridLineColor);
        GridMaterial->SetVectorParameterValue("WallColor", Colors.GridWallColor);
    }
}

void ASG_Grid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
  //  DrawGrid();
}

void ASG_Grid::DrawGrid()
{
    if (!GetWorld() || !GetWorld()->LineBatcher) return;

    for (uint32 i = 0; i < GridDim.height + 1; ++i)
    {
        const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * CellSize * i;
        // DrawDebugLine(GetWorld(), StartLocation, StartLocation + GetActorRightVector() * WorldWidth, FColor::Red, false, -1.0f, 0, 2.0f);
        GetWorld()->LineBatcher->DrawLine(StartLocation, StartLocation + GetActorRightVector() * WorldWidth, FLinearColor::Red, 0, 2.0f);
    }

    for (uint32 i = 0; i < GridDim.width + 1; ++i)
    {
        const FVector StartLocation = GetActorLocation() + GetActorRightVector() * CellSize * i;
        // DrawDebugLine(GetWorld(), StartLocation, StartLocation + GetActorForwardVector() * WorldHeight, FColor::Red, false, -1.0f,
        // 0, 2.0f);
        GetWorld()->LineBatcher->DrawLine(StartLocation, StartLocation + GetActorForwardVector() * WorldHeight, FLinearColor::Red, 0, 2.0f);
    }
}