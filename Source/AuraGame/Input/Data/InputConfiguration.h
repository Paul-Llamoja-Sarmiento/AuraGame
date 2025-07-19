
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfiguration.generated.h"


class UInputAction;

USTRUCT(BlueprintType)
struct FInputActionTagMapping
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

#if WITH_EDITOR
	// Validation check if all properties are set correctly
	EDataValidationResult IsDataValid(FDataValidationContext& Context, const int Index) const;
#endif
};

/**
 * 
 */
UCLASS()
class AURAGAME_API UInputConfiguration : public UDataAsset
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	// Validation check if InputActionMappings array is set correctly
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<FInputActionTagMapping> InputActionMappings;

	// UFUNCTION(BlueprintCallable, Category = "Input")
	// const UInputAction* FindInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
};
