// Copyright 2025, Rolling Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UILayerTypes.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWidgetLoaded, UUserWidget*, Widget);

UENUM(BlueprintType)
enum class EAsyncResultSwitch : uint8
{
    OnSuccess UMETA(DisplayName = "Success"),
    OnFailed UMETA(DisplayName = "Failed")
};
