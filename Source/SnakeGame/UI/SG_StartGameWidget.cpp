// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SG_StartGameWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>


void USG_StartGameWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    check(StartGameButton);
    StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);
}

void USG_StartGameWidget::OnStartGame() 
{
    if (!GameLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(GameLevel.GetAssetName()));
    }
}
