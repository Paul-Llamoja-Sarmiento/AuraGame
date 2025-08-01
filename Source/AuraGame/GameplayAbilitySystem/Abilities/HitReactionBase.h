
#pragma once

#include "CoreMinimal.h"
#include "ReactionAbilityBase.h"
#include "HitReactionBase.generated.h"


class ICharacterAssetProviderInterface;
class UGameplayEffect;


/**
 * @brief Reaction when the character takes damage.
 *
 // * Inherits from UReactionAbilityBase and handles:
 // * - Listening for the hit reaction tag/event (Effects.HitReaction).
 // * - Triggering hit reaction animation montages.
 // * - Optionally blocking movement or input during the reaction.
 */
UCLASS(Abstract)
class AURAGAME_API UHitReactionBase : public UReactionAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitReaction")
	TSubclassOf<UGameplayEffect> ReactionEffectClass;

	UPROPERTY(BlueprintReadOnly, Category = "HitReaction")
	FActiveGameplayEffectHandle ActiveEffectHandle;

	UPROPERTY(BlueprintReadOnly, Category = "HitReaction")
	TScriptInterface<ICharacterAssetProviderInterface> CharacterAssetProvider;
	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							 const FGameplayAbilityActivationInfo ActivationInfo,
							 const FGameplayEventData* TriggerEventData) override;

};
