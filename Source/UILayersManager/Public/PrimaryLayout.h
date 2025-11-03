// Copyright 2025, Rolling Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayout.generated.h"

class UUILayer;

UCLASS(Abstract)
class UILAYERSMANAGER_API UPrimaryLayout : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Define layers */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
    TMap<FGameplayTag, TSubclassOf<UUILayer>> LayerDefinitions;

    /** Define optional default widgets */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
    TMap<FGameplayTag, TSoftClassPtr<UUserWidget>> InitialWidgets;

    /** Initialize all layers for a player and push defaults */
    UFUNCTION(BlueprintCallable, Category = "Layout")
    void InitializeLayout(APlayerController* OwningPlayer);
};
