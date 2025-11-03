// Copyright 2025, Rolling Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UILayerTypes.generated.h"

UENUM(BlueprintType)
enum class EAsyncResultSwitch : uint8
{
    OnSuccess UMETA(DisplayName = "Success"),
    OnFailed UMETA(DisplayName = "Failed")
};
