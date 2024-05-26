// Fill out your copyright notice in the Description page of Project Settings.

#include "World/SG_Snake.h"
#include "World/SG_WorldTypes.h"
#include "SG_SnakeLink.h"
#include "SG_WorldUtils.h"

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::Dimensions& InDims)
{
    Snake = InSnake;
    CellSize = InCellSize;
    Dims = InDims;

    for (auto* LinkActor : SnakeLinks)
    {
        LinkActor->Destroy();
    }
    SnakeLinks.Empty();

    if (!Snake.IsValid() || !GetWorld())
    {
        return;
    }

    const auto& Links = Snake.Pin()->getLinks();

    uint32 i = 0;
    for (const auto& Link : Links)
    {
        const bool IsHead = i == 0;
        const FTransform Transform = FTransform(SnakeGame::WorldUtils::LinkPositionToVector(Link, CellSize, Dims));
        auto* LinkActor = GetWorld()->SpawnActorDeferred<ASG_SnakeLink>(IsHead ? SnakeHeadClass : SnakeLinkClass, Transform);
        LinkActor->UpdateScale(CellSize);
        LinkActor->FinishSpawning(Transform);
        SnakeLinks.Add(LinkActor);
        ++i;
    }
}

void ASG_Snake::UpdateColor(const FSnakeColors& Colors)
{
    SnakeLinkColor = Colors.SnakeLinkColor;
    for (int32 i = 0; i < SnakeLinks.Num(); ++i)
    {
        SnakeLinks[i]->UpdateColor(i == 0 ? Colors.SnakeHeadColor : Colors.SnakeLinkColor);
    }
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Snake.IsValid())
    {
        return;
    }

    const auto& Links = Snake.Pin()->getLinks();
    auto* LinkPtr = Links.GetHead();

    for (auto* LinkActor : SnakeLinks)
    {
        LinkActor->SetActorLocation(SnakeGame::WorldUtils::LinkPositionToVector(LinkPtr->GetValue(), CellSize, Dims));
        LinkPtr = LinkPtr->GetNextNode();
    }

    while (LinkPtr)
    {
        const FTransform Transform = FTransform(SnakeGame::WorldUtils::LinkPositionToVector(LinkPtr->GetValue(), CellSize, Dims));
        auto* LinkActor = GetWorld()->SpawnActorDeferred<ASG_SnakeLink>(SnakeLinkClass, Transform);
        LinkActor->UpdateScale(CellSize);
        LinkActor->UpdateColor(SnakeLinkColor);
        LinkActor->FinishSpawning(Transform);
        SnakeLinks.Add(LinkActor);
        LinkPtr = LinkPtr->GetNextNode();
    }
}

void ASG_Snake::Explode()
{
    for (auto* LinkActor : SnakeLinks)
    {
        LinkActor->Explode();
    }
}
