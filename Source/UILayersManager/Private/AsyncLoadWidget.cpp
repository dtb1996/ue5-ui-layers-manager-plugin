// Fill out your copyright notice in the Description page of Project Settings.

#include "AsyncLoadWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UILayersManagerSubsystem.h"
#include "UILayersManager.h"

UAsyncLoadWidget* UAsyncLoadWidget::PushToLayerAsync(APlayerController* OwningPlayer, const FGameplayTag& LayerTag, TSoftClassPtr<UUserWidget> WidgetClass)
{
	UAsyncLoadWidget* AsyncTask = NewObject<UAsyncLoadWidget>();
	if (!AsyncTask)
	{
		UE_LOG(LogUILayersManager, Error, TEXT("PushToLayerAsync: Failed to create AsyncTask object."));
		return nullptr;
	}

	AsyncTask->OwningPlayer = OwningPlayer;
	AsyncTask->LayerTag = LayerTag;
	AsyncTask->WidgetClassRef = WidgetClass;
	
	return AsyncTask;
}

void UAsyncLoadWidget::Activate()
{
	Super::Activate();

	if (!OwningPlayer.IsValid())
	{
		UE_LOG(LogUILayersManager, Warning, TEXT("Activate: OwningPlayer is invalid."));
		OnCompleted.Broadcast(nullptr);
		return;
	}

	if (!WidgetClassRef.IsValid() && WidgetClassRef.ToSoftObjectPath().IsNull())
	{
		UE_LOG(LogUILayersManager, Warning, TEXT("Activate: WidgetClassRef is invalid or null."));
		OnCompleted.Broadcast(nullptr);
		return;
	}

	// If already loaded, skip async path
	if (UClass* WidgetClass = WidgetClassRef.Get())
	{
		UE_LOG(LogUILayersManager, Verbose, TEXT("Activate: Widget already loaded, skipping async load."));
		OnWidgetClassReady(WidgetClass);
		return;
	}

	// Async load
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	UE_LOG(LogUILayersManager, Verbose, TEXT("Activate: Starting async load for widget: %s"), *WidgetClassRef.ToSoftObjectPath().ToString());

	StreamableManager.RequestAsyncLoad(
		WidgetClassRef.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &UAsyncLoadWidget::OnWidgetClassLoaded)
	);
}

void UAsyncLoadWidget::OnWidgetClassReady(UClass* WidgetClass)
{
	if (!OwningPlayer.IsValid())
	{
		UE_LOG(LogUILayersManager, Warning, TEXT("OnWidgetClassReady: OwningPlayer is invalid."));
		OnCompleted.Broadcast(nullptr);
		return;
	}

	if (!WidgetClass)
	{
		UE_LOG(LogUILayersManager, Warning, TEXT("OnWidgetClassReady: WidgetClass is null."));
		OnCompleted.Broadcast(nullptr);
		return;
	}

	APlayerController* PC = OwningPlayer.Get();
	if (!PC)
	{
		UE_LOG(LogUILayersManager, Warning, TEXT("OnWidgetClassReady: PlayerController is null."));
		OnCompleted.Broadcast(nullptr);
		return;
	}

	if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
	{
		if (UUILayersManagerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UUILayersManagerSubsystem>())
		{
			UE_LOG(LogUILayersManager, Verbose, TEXT("OnWidgetClassReady: Pushing widget to layer: %s"),
				*LayerTag.ToString());

			UUserWidget* Widget = Subsystem->PushToLayer(LayerTag, WidgetClass);
			OnCompleted.Broadcast(Widget);
			return;
		}
		else
		{
			UE_LOG(LogUILayersManager, Warning, TEXT("OnWidgetClassReady: UILayersManagerSubsystem not found."));
		}
	}
	else
	{
		UE_LOG(LogUILayersManager, Warning, TEXT("OnWidgetClassReady: LocalPlayer is null."));
	}

	// Fallback if subsystem is missing
	UE_LOG(LogUILayersManager, Warning, TEXT("OnWidgetClassReady: Falling back to CreateWidget."));

	UUserWidget* Widget = CreateWidget<UUserWidget>(PC, WidgetClass);

	if (!Widget)
	{
		UE_LOG(LogUILayersManager, Error, TEXT("OnWidgetClassReady: CreateWidget failed."));
	}

	OnCompleted.Broadcast(Widget);
}

void UAsyncLoadWidget::OnWidgetClassLoaded()
{
	if (!OwningPlayer.IsValid())
	{
		UE_LOG(LogUILayersManager, Warning, TEXT("OnWidgetClassLoaded: OwningPlayer is invalid."));
		OnCompleted.Broadcast(nullptr);
		return;
	}

	if (UClass* WidgetClass = WidgetClassRef.Get())
	{
		UE_LOG(LogUILayersManager, Verbose, TEXT("OnWidgetClassLoaded: Widget class loaded successfully."));
		OnWidgetClassReady(WidgetClass);
	}
	else
	{
		UE_LOG(LogUILayersManager, Error, TEXT("OnWidgetClassLoaded: Failed to resolve WidgetClass after async load: %s"), *WidgetClassRef.ToSoftObjectPath().ToString());
		OnCompleted.Broadcast(nullptr);
	}
}
