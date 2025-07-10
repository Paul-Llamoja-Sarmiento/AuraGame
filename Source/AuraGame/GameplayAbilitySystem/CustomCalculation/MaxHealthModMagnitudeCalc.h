
#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MaxHealthModMagnitudeCalc.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAME_API UMaxHealthModMagnitudeCalc : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMaxHealthModMagnitudeCalc();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDefinition;
};
