// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
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
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category="UI"))
	static UAsyncLoadWidget* AsyncLoadWidget(UObject* WorldContextObject, TSoftClassPtr<UUserWidget> WidgetClass);

	virtual void Activate() override;
private:
	UPROPERTY()
	UObject* WorldContextObject;
	
	UPROPERTY()
	TSoftClassPtr<UUserWidget> WidgetClassRef;

	void OnWidgetClassLoaded();
};