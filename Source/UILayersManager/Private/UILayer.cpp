// Copyright 2025, Rolling Pixels. All Rights Reserved

#include "UILayer.h"
#include "Components/Border.h"
#include "UILayersManager.h"

void UUILayer::PushContent(TSoftClassPtr<UUserWidget> WidgetClass, FOnWidgetLoaded Callback)
{
    CallbackRef = Callback;

    if (WidgetClass.IsNull())
    {
        UE_LOG(LogUILayersManager, Error, TEXT("PushContent: WidgetClass is null"));

        if (CallbackRef.IsBound())
        {
            CallbackRef.Execute(nullptr);
            CallbackRef.Clear();
        }

        return;
    }
        
    RequestAsyncLoadWidget(WidgetClass);
}

void UUILayer::PopContent()
{
    if (Stack.Num() == 0)
    {
        return;
    }

    if (UUserWidget* Top = Peek())
    {
        Top->RemoveFromParent();
        Stack.Pop();
        Border->ClearChildren();
    }

    if (UUserWidget* NewTop = Peek())
    {
        NewTop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        Border->AddChild(NewTop);
    }
}

void UUILayer::ClearStack()
{
    for (UUserWidget* Widget : Stack)
    {
        if (Widget)
        {
            Widget->RemoveFromParent();
        }
    }

    Stack.Empty();
    Border->ClearChildren();
}

UUserWidget* UUILayer::Peek() const
{
    return Stack.IsEmpty() ? nullptr : Stack.Last();
}

void UUILayer::CollapseTop()
{
    if (UUserWidget* Widget = Peek())
    {
        Widget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UUILayer::ShowTop()
{
    if (UUserWidget* Widget = Peek())
    {
        Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

FText UUILayer::GetStackListNames() const
{
    TArray<FString> Names;
    for (UUserWidget* Widget : Stack)
    {
        Names.Push(Widget ? Widget->GetName() : TEXT("None"));
    }

    return FText::FromString(FString::Join(Names, TEXT("\n")));
}

bool UUILayer::IsEmpty() const
{
    return Stack.Num() == 0;
}

void UUILayer::OnWidgetLoaded(UUserWidget* LoadedWidget)
{
    if (!LoadedWidget)
    {
        return;
    }

    CollapseTop();
    Border->ClearChildren();

    Stack.Add(LoadedWidget);
    Border->AddChild(LoadedWidget);
    ShowTop();
    
    if (CallbackRef.IsBound())
    {
        CallbackRef.Execute(LoadedWidget);
        CallbackRef.Clear();
    }
}

