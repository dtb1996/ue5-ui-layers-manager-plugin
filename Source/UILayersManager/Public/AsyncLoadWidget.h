// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "AsyncLoadWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadCompleted, UUserWidget*, LoadedWidget);

UCLASS()
class UILAYERSMANAGER_API UAsyncLoadWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FLoadCompleted OnCompleted;
	
	// Factory function
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category="UI Layers|Async"))
	static UAsyncLoadWidget* PushToLayerAsync(APlayerController* OwningPlayer, UObject* WorldContextObject, FGameplayTag LayerTag, TSoftClassPtr<UUserWidget> WidgetClass);

	virtual void Activate() override;
private:
	APlayerController* OwningPlayer;
	UObject* WorldContextObject;
	FGameplayTag LayerTag;
	TSoftClassPtr<UUserWidget> WidgetClassRef;

	void OnWidgetClassReady(UClass* WidgetClass);
	void OnWidgetClassLoaded();
};