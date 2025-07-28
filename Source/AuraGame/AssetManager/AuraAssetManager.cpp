
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
	
	/**
	 * Between UE 4.24 and 5.2, it is necessary to call UAbilitySystemGlobals::Get().InitGlobalData() to use TargetData,
	 * otherwise you will get errors related to ScriptStructCache and clients will be disconnected from the server.
	 * This function only needs to be called once in a project. Fortnite calls it from UAssetManager::StartInitialLoading()
	 * and Paragon called it from UEngine::Init(). I find that putting it in UAssetManager::StartInitialLoading()
	 * is a good place as shown in the Sample Project. I would consider this boilerplate code that you should copy into
	 * your project to avoid issues with TargetData. Starting in 5.3 it is called automatically.
	 *
	 * If you run into a crash while using the AbilitySystemGlobals GlobalAttributeSetDefaultsTableNames,
	 * you may need to call UAbilitySystemGlobals::Get().InitGlobalData() later like Fortnite in the AssetManager
	 * or in the GameInstance.
	 */
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
