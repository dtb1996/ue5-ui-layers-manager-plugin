// Copyright 2025, Rolling Pixels. All Rights Reserved.

#include "PrimaryLayout.h"
#include "UILayer.h"
#include "UILayersManagerSubsystem.h"
#include "UILayersManager.h"

void UPrimaryLayout::InitializeLayout(APlayerController* OwningPlayer)
{
    if (!OwningPlayer)
    {
        return;
    }

    UUILayersManagerSubsystem* Subsystem = OwningPlayer->GetGameInstance()->GetSubsystem<UUILayersManagerSubsystem>();
    if (!Subsystem) 
    {
        return;
    }

    // Create layers
    for (auto& Pair : LayerDefinitions)
    {
        FGameplayTag LayerTag = Pair.Key;
        TSubclassOf<UUILayer> LayerClass = Pair.Value;

        if (!LayerClass)
        {
            continue;
        }

        Subsystem->CreateLayer(LayerTag, LayerClass);
    }

    // Push default widgets
    for (auto& Pair : InitialWidgets)
    {
        if (!Pair.Value.IsNull())
        {
            Subsystem->PushToLayer(Pair.Key, Pair.Value);
        }
    }

    UE_LOG(LogUILayersManager, Log, TEXT("PrimaryLayout initialized %d layers with defaults"), LayerDefinitions.Num());
}
