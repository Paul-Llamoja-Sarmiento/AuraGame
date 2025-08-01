
#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "DamageAbilityBase.generated.h"


/**
 * @brief Ability types specialized in dealing damage.
 *
 * Abstract base for abilities initiated directly by player input that deals damage.
 *
 * Examples of subclasses:
 * - UProjectileSpellBase: casts a magic projectile that explodes on impact.
 * - UMeleeAttackBase: performs close-range hits with a weapon.
 */
UCLASS(Abstract)
class AURAGAME_API UDamageAbilityBase : public UGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat BaseDamage = 1.f;
};
