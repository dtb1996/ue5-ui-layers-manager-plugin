// Copyright 2025, Rolling Pixels. All Rights Reserved.

#include "UIHUD.h"
#include "PrimaryLayout.h"
#include "UILayer.h"
#include "UILayersManager.h"

void AUIHUD::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = GetOwningPlayerController();
    if (!PC)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("AUIHUD: PlayerController not found"));
        return;
    }

    if (!PrimaryLayoutClass)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("AUIHUD: PrimaryLayoutClass is not set"));
        return;
    }

    PrimaryLayout = CreateWidget<UPrimaryLayout>(PC, PrimaryLayoutClass);
    if (!PrimaryLayout)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("Failed to create PrimaryLayout."));
        return;
    }

    PrimaryLayout->InitializeLayout(PC, LayerDefinitions, InitialWidgets);
    PrimaryLayout->AddToViewport();
}
