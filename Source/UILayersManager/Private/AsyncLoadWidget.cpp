// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncLoadWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

UAsyncLoadWidget* UAsyncLoadWidget::AsyncLoadWidget(UObject* WorldContextObject, TSoftClassPtr<UUserWidget> WidgetClass)
{
	UAsyncLoadWidget* AsyncTask = NewObject<UAsyncLoadWidget>();
	AsyncTask->WorldContextObject = WorldContextObject;
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

	// Try to get a valid world or game instance
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
		return;
	}

	// Async load
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(
		WidgetClassRef.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UAsyncLoadWidget::OnWidgetClassLoaded)
	);
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
