
#include "AuraAssetManager.h"

#include "AuraGame/AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAttributeGameplayTags::InitializeNativeGasGameplayTags();
}
