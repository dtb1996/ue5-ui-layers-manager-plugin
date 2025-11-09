// Copyright 2025, Rolling Pixels. All Rights Reserved.

#include "UIHUD.h"
#include "UILayersManagerSubsystem.h"
#include "UILayer.h"
#include "UILayersManager.h"

void AUIHUD::BeginPlay()
{
    Super::BeginPlay();

    InitializeLayout();
}

void AUIHUD::InitializeLayout()
{
    APlayerController* PC = GetOwningPlayerController();
    if (!PC)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("InitializeLayout: PlayerController not found"));
        return;
    }

    ULocalPlayer* LP = PC->GetLocalPlayer();
    if (!LP)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("InitializeLayout: Local Player not found"));
        return;
    }

    UUILayersManagerSubsystem* Subsystem = LP->GetSubsystem<UUILayersManagerSubsystem>();
    if (!Subsystem)
    {
        return;
    }

    // Create layers
    for (auto& Pair : LayerDefinitions)
    {
        if (!Pair.Value)
        {
            continue;
        }

        Subsystem->CreateLayer(Pair.Key, Pair.Value);
    }

    // Push default widgets
    for (auto& Pair : InitialWidgets)
    {
        if (!Pair.Value.IsNull())
        {
            Subsystem->PushToLayerWithCallback(Pair.Key, Pair.Value, FOnWidgetLoaded());
        }
    }

    UE_LOG(LogUILayersManager, Log, TEXT("Initialized %d layers with default widgets"), LayerDefinitions.Num());
}
