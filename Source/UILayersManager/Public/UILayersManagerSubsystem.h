// Copyright 2025, Rolling Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "GameplayTagContainer.h"
#include "UILayerTypes.h"
#include "UILayersManagerSubsystem.generated.h"

class UUILayer;

UCLASS()
class UILAYERSMANAGER_API UUILayersManagerSubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    /** Get subsystem from any WorldContextObject */
    static UUILayersManagerSubsystem* Get(const UObject* WorldContextObject);

    /** Create or retrieve a layer */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    UUILayer* CreateLayer(FGameplayTag LayerTag, TSubclassOf<UUILayer> LayerClass);

    /** Pushes a widget onto a specific layer */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    UUserWidget* PushToLayer(FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass);

    /** Pushes a widget onto a specific layer. Includes an optional callback function pin to get a reference to the created widget */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    void PushToLayerWithCallback(FGameplayTag LayerTag, TSoftClassPtr<UUserWidget> WidgetClass, FOnWidgetLoaded Callback);

    /** Pops a widget from a specific layer if found */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    void PopFromLayer(FGameplayTag LayerTag);

    /** Clears all widgets in a specific layer if found */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    void ClearLayer(FGameplayTag LayerTag);

    /** Clears all widgets all found layers */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    void ClearAllLayers();

    /** Clears all widgets in all layers except the one specified */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    void ClearAllLayersExcept(FGameplayTag ExceptionLayerTag);

    /** Retrieve an existing layer */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    UUILayer* GetLayer(FGameplayTag LayerTag) const;

    /** Remove a layer */
    UFUNCTION(BlueprintCallable, Category = "UI Layers")
    void RemoveLayer(FGameplayTag LayerTag);

private:
    /** All active layers, keyed by tag */
    UPROPERTY(Transient)
    TMap<FGameplayTag, UUILayer*> ActiveLayers;
};
