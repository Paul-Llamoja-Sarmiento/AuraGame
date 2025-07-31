
#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "DamageAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAME_API UDamageAbilityBase : public UGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat BaseDamage = 1.f;
};
