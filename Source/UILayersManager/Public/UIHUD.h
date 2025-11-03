// Copyright 2025, Rolling Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIHUD.generated.h"

class UPrimaryLayout;

UCLASS(Abstract)
class AUIHUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UPrimaryLayout> LayoutClass;

protected:
    UPROPERTY()
    UPrimaryLayout* ActiveLayout;

    virtual void BeginPlay() override;
};
