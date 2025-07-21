
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"


class UHighlightingComponent;
class UAutoRunComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UInputConfiguration;
class UInputAction;
class UInputMappingContext;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerBase();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputConfiguration> InputConfiguration;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float ShortPressThresholdInSeconds = 0.2f;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetAuraASC();

	// Input handling functions
	void Move(const FInputActionValue& InputActionValue);
	void AbilityInputPressed(const FGameplayTag InputTag);
	void AbilityInputHeld(const FGameplayTag InputTag);
	void AbilityInputReleased(const FGameplayTag InputTag);

	// Cursor movement
	float FollowTime = 0.f;
	bool bIsTargeting = false;

	void HandleDirectMovementInput();

	void ConfigureInputMode();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAutoRunComponent> AutoRunComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHighlightingComponent> HighlightingComponent;
};
