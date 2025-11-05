// Copyright 2025, Rolling Pixels. All Rights Reserved.

#include "PrimaryLayout.h"
#include "UILayer.h"
#include "UILayersManagerSubsystem.h"
#include "UILayersManager.h"

void UPrimaryLayout::InitializeLayout(APlayerController* OwningPlayer, TMap<FGameplayTag, TSubclassOf<UUILayer>> LayerDefinitions, TMap<FGameplayTag, TSoftClassPtr<UUserWidget>> InitialWidgets)
{
    if (!OwningPlayer)
    {
        return;
    }

    ULocalPlayer* LP = OwningPlayer->GetLocalPlayer();
    if (!LP)
    {
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

    UE_LOG(LogUILayersManager, Log, TEXT("PrimaryLayout initialized %d layers with defaults"), LayerDefinitions.Num());
}
