
#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "ProjectileSpellBase.generated.h"


class AProjectileBase;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API UProjectileSpellBase : public UGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile() const;
};
