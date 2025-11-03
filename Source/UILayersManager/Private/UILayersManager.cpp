// Copyright 2025, Rolling Pixels. All Rights Reserved.

#include "UILayersManager.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"
#include "UILayersManagerSubsystem.h"
#include "GameplayTagsManager.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogUILayersManager);

#define LOCTEXT_NAMESPACE "FUILayersManagerModule"

void FUILayersManagerModule::StartupModule()
{
    UE_LOG(LogUILayersManager, Log, TEXT("UILayersManager module starting up..."));

	UGameplayTagsManager::Get().AddTagIniSearchPath(FPaths::ProjectPluginsDir() / TEXT("UILayersManager/Config/Tags"));

#if WITH_EDITOR
    UE_LOG(LogUILayersManager, Verbose, TEXT("Running in Editor mode."));
#endif

    if (GEngine && GEngine->GameViewport)
    {
        if (UGameInstance* GI = GEngine->GameViewport->GetGameInstance())
        {
            if (GI->GetSubsystemBase(UUILayersManagerSubsystem::StaticClass()))
            {
                UE_LOG(LogUILayersManager, Log, TEXT("UILayersManagerSubsystem found and active."));
            }
            else
            {
                UE_LOG(LogUILayersManager, Warning, TEXT("UILayersManagerSubsystem not found on startup."));
            }
        }
    }

}

void FUILayersManagerModule::ShutdownModule()
{
	UE_LOG(LogUILayersManager, Log, TEXT("UILayersManager module shutting down..."));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUILayersManagerModule, UILayersManager)