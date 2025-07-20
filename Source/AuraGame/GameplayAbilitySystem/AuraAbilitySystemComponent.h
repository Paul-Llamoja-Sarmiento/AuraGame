
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAuraAbilitySystemComponent();

	void GrantCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>&  StartupAbilities);


	// Input handling functions
	void AbilityInputHeldHandle(const FGameplayTag& InputTag);
	void AbilityInputReleasedHandle(const FGameplayTag& InputTag);
};
