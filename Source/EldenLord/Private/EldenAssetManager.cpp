// luce copyright


#include "EldenAssetManager.h"
#include "EldenGameplayTags.h"

UEldenAssetManager& UEldenAssetManager::Get()
{
	checkf(GEngine,TEXT("GEngine is null"));
	UEldenAssetManager* EldenAssetManager = Cast<UEldenAssetManager>(GEngine->AssetManager);
	return *EldenAssetManager;
}

void UEldenAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FEldenGameplayTags::InitializeNativeGameplayTags();
}
