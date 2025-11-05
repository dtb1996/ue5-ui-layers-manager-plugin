// Copyright 2025, Rolling Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "UILayerTypes.h"
#include "UILayer.generated.h"

UCLASS(Abstract)
class UILAYERSMANAGER_API UUILayer : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Layer")
    FGameplayTag LayerTag;

    UUserWidget* PushContent(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    void PushContent(TSoftClassPtr<UUserWidget> WidgetClass, FOnWidgetLoaded Callback);

    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    void PopContent();

    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    void ClearStack();

    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    UUserWidget* Peek() const;

    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    void CollapseTop();

    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    void ShowTop();

    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    FText GetStackListNames() const;

    bool IsEmpty() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    class UBorder* Border;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI Layer")
    TArray<UUserWidget*> Stack;

    /** Blueprint implements the async loading process */
    UFUNCTION(BlueprintImplementableEvent, Category = "UI Layer")
    void RequestAsyncLoadWidget(const TSoftClassPtr<UUserWidget>& WidgetClass);

    /** Called when async load finishes */
    UFUNCTION(BlueprintCallable, Category = "UI Layer")
    void OnWidgetLoaded(UUserWidget* LoadedWidget);

private:
    FOnWidgetLoaded CallbackRef;
};
