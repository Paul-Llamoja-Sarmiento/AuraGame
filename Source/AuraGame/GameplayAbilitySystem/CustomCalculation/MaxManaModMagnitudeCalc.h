
#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MaxManaModMagnitudeCalc.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAME_API UMaxManaModMagnitudeCalc : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMaxManaModMagnitudeCalc();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDefinition;
};
