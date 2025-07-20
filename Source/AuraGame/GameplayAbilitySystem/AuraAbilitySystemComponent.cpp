
#include "AuraAbilitySystemComponent.h"

#include "Abilities/GameplayAbilityBase.h"


UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAuraAbilitySystemComponent::GrantCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto& Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		if (const UGameplayAbilityBase* AbilityBase = CastChecked<UGameplayAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityBase->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputHeldHandle(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) continue;
		
		AbilitySpecInputPressed(AbilitySpec);
		if (!AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputReleasedHandle(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) continue;
		
		AbilitySpecInputReleased(AbilitySpec);
	}
}

