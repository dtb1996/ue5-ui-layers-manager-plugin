// Copyright 2025, Rolling Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayTagContainer.h"
#include "UIHUD.generated.h"

class UUILayer;

UCLASS(Abstract)
class AUIHUD : public AHUD
{
    GENERATED_BODY()

protected:
	/** Define layers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TMap<FGameplayTag, TSubclassOf<UUILayer>> LayerDefinitions;

	/** Define optional default widgets */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TMap<FGameplayTag, TSoftClassPtr<UUserWidget>> InitialWidgets;

    virtual void BeginPlay() override;

private:
	void InitializeLayout();
};
