// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncLoadWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UILayersManagerSubsystem.h"

UAsyncLoadWidget* UAsyncLoadWidget::PushToLayerAsync(APlayerController* OwningPlayer, UObject* WorldContextObject, FGameplayTag LayerTag, TSoftClassPtr<UUserWidget> WidgetClass)
{
	UAsyncLoadWidget* AsyncTask = NewObject<UAsyncLoadWidget>();
	AsyncTask->OwningPlayer = OwningPlayer;
	AsyncTask->WorldContextObject = WorldContextObject;
	AsyncTask->LayerTag = LayerTag;
	AsyncTask->WidgetClassRef = WidgetClass;
	
	return AsyncTask;
}

void UAsyncLoadWidget::Activate()
{
	Super::Activate();

	if (!WorldContextObject || !WidgetClassRef.IsValid())
	{
		OnCompleted.Broadcast(nullptr);
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World)
	{
		OnCompleted.Broadcast(nullptr);
		return;
	}

	// If already loaded, skip async path
	if (UClass* WidgetClass = WidgetClassRef.Get())
	{
		OnWidgetClassReady(WidgetClass);
		return;
	}

	// Async load
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(
		WidgetClassRef.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UAsyncLoadWidget::OnWidgetClassLoaded)
	);
}

void UAsyncLoadWidget::OnWidgetClassReady(UClass* WidgetClass)
{
	if (!OwningPlayer || !WidgetClass)
	{
		OnCompleted.Broadcast(nullptr);
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World)
	{
		OnCompleted.Broadcast(nullptr);
		return;
	}

	if (ULocalPlayer* LocalPlayer = OwningPlayer->GetLocalPlayer())
	{
		if (UUILayersManagerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UUILayersManagerSubsystem>())
		{
			UUserWidget* Widget = Subsystem->PushToLayer(LayerTag, WidgetClass);
			OnCompleted.Broadcast(Widget);
			return;
		}
	}

	// Fallback if subsystem is missing
	UUserWidget* Widget = CreateWidget<UUserWidget>(World, WidgetClass);
	OnCompleted.Broadcast(Widget);
}

void UAsyncLoadWidget::OnWidgetClassLoaded()
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World)
	{
		OnCompleted.Broadcast(nullptr);
		return;
	}

	if (UClass* WidgetClass = WidgetClassRef.Get())
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(World, WidgetClass);
		OnCompleted.Broadcast(Widget);
	}
	else
	{
		OnCompleted.Broadcast(nullptr);
	}
}
