
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectTagContainerReceived, const FGameplayTagContainer& /*AssetTags*/);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAuraAbilitySystemComponent();

	void OnAbilityActorInfoSet();

	void GrantCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>&  StartupAbilities);

	FOnEffectTagContainerReceived OnEffectTagContainerReceived;

	// Input handling functions
	void AbilityInputHeldHandle(const FGameplayTag& InputTag);
	void AbilityInputReleasedHandle(const FGameplayTag& InputTag);

private:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
