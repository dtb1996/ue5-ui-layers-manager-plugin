// Copyright 2025, Rolling Pixels. All Rights Reserved.

#include "UIHUD.h"
#include "PrimaryLayout.h"
#include "UILayersManagerSubsystem.h"

void AUIHUD::BeginPlay()
{
    Super::BeginPlay();

    if (!LayoutClass)
    {
        return;
    }

    APlayerController* PC = GetOwningPlayerController();
    if (!PC)
    {
        return;
    }

    ActiveLayout = CreateWidget<UPrimaryLayout>(PC, LayoutClass);
    if (!ActiveLayout)
    {
        return;
    }

    ActiveLayout->AddToViewport();
    ActiveLayout->InitializeLayout(PC);
}
