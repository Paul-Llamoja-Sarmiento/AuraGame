
#include "HitReactionBase.h"

#include "AuraGame/Interaction/CharacterAssetProviderInterface.h"

void UHitReactionBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	if (CharacterAssetProvider == nullptr)
	{
		CharacterAssetProvider = TScriptInterface<ICharacterAssetProviderInterface>(ActorInfo->AvatarActor.Get());
	}
	
	const UGameplayEffect* GameplayEffect = ReactionEffectClass->GetDefaultObject<UGameplayEffect>();
	ActiveEffectHandle = ApplyGameplayEffectToOwner(
		GetCurrentAbilitySpecHandle(), ActorInfo, ActivationInfo, GameplayEffect, GetAbilityLevel());

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
