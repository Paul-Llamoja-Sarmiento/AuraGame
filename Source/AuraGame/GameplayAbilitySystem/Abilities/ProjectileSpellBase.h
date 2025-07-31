
#pragma once

#include "CoreMinimal.h"
#include "DamageAbilityBase.h"
#include "ProjectileSpellBase.generated.h"


class AProjectileBase;
class UGameplayEffect;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API UProjectileSpellBase : public UDamageAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> SpellEffectClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
};
