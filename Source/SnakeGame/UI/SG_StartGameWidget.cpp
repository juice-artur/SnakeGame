// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SG_StartGameWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include "Components/ComboBoxString.h"
#include "Framework/SG_GameUserSettings.h"

void USG_StartGameWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = USG_GameUserSettings::Get();
    if (!UserSettings) return;

    check(StartGameButton);
    StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);

    InitComboBox(
        GameSpeedComboBox,                                   //
        [&]() { return UserSettings->GameSpeedOptions(); },  //
        [&]() { return UserSettings->CurrentGameSpeedOption(); });

    InitComboBox(
        GridSizeComboBox,                                   //
        [&]() { return UserSettings->GridSizeOptions(); },  //
        [&]() { return UserSettings->CurrentGridSizeOption(); });
}

void USG_StartGameWidget::OnStartGame()
{
    if (!GameLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(GameLevel.GetAssetName()));
    }
}

void USG_StartGameWidget::OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) 
{
    SaveSettings();
}

void USG_StartGameWidget::InitComboBox(
    const TObjectPtr<UComboBoxString>& ComboBox, TFunction<TArray<FString>()> OptionsGetter, TFunction<FString()> CurrentOptionGetter)
{
    check(ComboBox);
    ComboBox->ClearOptions();
    for (const auto& Option : OptionsGetter())
    {
        ComboBox->AddOption(Option);
    }
    ComboBox->SetSelectedOption(CurrentOptionGetter());
    ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnSelectionChanged);
}

void USG_StartGameWidget::SaveSettings()
{
    if (auto* UserSettings = USG_GameUserSettings::Get())
    {
        const EGameSpeed GameSpeed = UserSettings->GameSpeedByName(GameSpeedComboBox->GetSelectedOption());
        const EGridSize GridSize = UserSettings->GridSizeByName(GridSizeComboBox->GetSelectedOption());
        UserSettings->SaveSnakeSettings(GameSpeed, GridSize);
    }
}
