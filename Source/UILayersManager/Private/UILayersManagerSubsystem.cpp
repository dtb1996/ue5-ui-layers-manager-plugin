// Copyright 2025, Rolling Pixels. All Rights Reserved.

#include "UILayersManagerSubsystem.h"
#include "UILayer.h"
#include "Engine/World.h"
#include "UILayersManager.h"

UUILayersManagerSubsystem* UUILayersManagerSubsystem::Get(const UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    if (UWorld* World = WorldContextObject->GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            if (ULocalPlayer* LP = PC->GetLocalPlayer())
            {
                return LP->GetSubsystem<UUILayersManagerSubsystem>();
            }
        }
    }

    return nullptr;
}

UUILayer* UUILayersManagerSubsystem::CreateLayer(FGameplayTag LayerTag, TSubclassOf<UUILayer> LayerClass)
{
    if (!LayerClass)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("CreateLayer: Invalid LayerClass for tag %s"), *LayerTag.ToString());
        return nullptr;
    }

    if (ActiveLayers.Contains(LayerTag))
    {
        UE_LOG(LogUILayersManager, Verbose, TEXT("Layer %s already exists"), *LayerTag.ToString());
        return ActiveLayers[LayerTag];
    }

    if (UWorld* World = GetWorld())
    {
        UUILayer* NewLayer = CreateWidget<UUILayer>(World, LayerClass);
        if (NewLayer)
        {
            NewLayer->LayerTag = LayerTag;
            NewLayer->AddToViewport();
            ActiveLayers.Add(LayerTag, NewLayer);
            UE_LOG(LogUILayersManager, Log, TEXT("Created layer %s"), *LayerTag.ToString());
            return NewLayer;
        }
    }

    UE_LOG(LogUILayersManager, Error, TEXT("Failed to create layer %s"), *LayerTag.ToString());
    return nullptr;
}

UUserWidget* UUILayersManagerSubsystem::PushToLayer(FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass)
{
    UUILayer* Layer = GetLayer(LayerTag);
    if (!Layer)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("PushToLayer: Layer %s not found"), *LayerTag.ToString());
        return nullptr;
    }

    return Layer->PushContent(WidgetClass);
}

void UUILayersManagerSubsystem::PushToLayerWithCallback(FGameplayTag LayerTag, TSoftClassPtr<UUserWidget> WidgetClass, FOnWidgetLoaded Callback)
{
    UUILayer* Layer = GetLayer(LayerTag);
    if (!Layer)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("PushToLayerWithCallback: Layer %s not found"), *LayerTag.ToString());
        return;
    }

    Layer->PushContent(WidgetClass, Callback);
}

void UUILayersManagerSubsystem::PopFromLayer(FGameplayTag LayerTag)
{
    UUILayer* Layer = GetLayer(LayerTag);
    if (!Layer)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("PopFromLayer: Layer %s not found"), *LayerTag.ToString());
        return;
    }

    Layer->PopContent();
}

void UUILayersManagerSubsystem::ClearLayer(FGameplayTag LayerTag)
{
    UUILayer* Layer = GetLayer(LayerTag);
    if (!Layer)
    {
        UE_LOG(LogUILayersManager, Warning, TEXT("ClearLayer: Layer %s not found"), *LayerTag.ToString());
        return;
    }

    Layer->ClearStack();
}

void UUILayersManagerSubsystem::ClearAllLayers()
{
    for (auto& Pair : ActiveLayers)
    {
        UUILayer* Layer = Pair.Value;

        if (!Layer)
        {
            continue;
        }

        Layer->ClearStack();
    }
}

void UUILayersManagerSubsystem::ClearAllLayersExcept(FGameplayTag ExceptionLayerTag)
{
    for (auto& Pair : ActiveLayers)
    {
        UUILayer* Layer = Pair.Value;

        if (!Layer || Layer->LayerTag == ExceptionLayerTag)
        {
            continue;
        }

        Layer->ClearStack();
    }
}

UUILayer* UUILayersManagerSubsystem::GetLayer(FGameplayTag LayerTag) const
{
    if (UUILayer* const* Found = ActiveLayers.Find(LayerTag))
    {
        return *Found;
    }

    return nullptr;
}

void UUILayersManagerSubsystem::RemoveLayer(FGameplayTag LayerTag)
{
    if (UUILayer* Layer = GetLayer(LayerTag))
    {
        Layer->RemoveFromParent();
        ActiveLayers.Remove(LayerTag);
        UE_LOG(LogUILayersManager, Log, TEXT("Removed layer %s"), *LayerTag.ToString());
    }
}