
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"


class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UInputConfiguration;
class IHighlightableActor;
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

	virtual void PlayerTick(float DeltaTime) override;

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

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY()
	TScriptInterface<IHighlightableActor> LastHighlightedActor;

	UPROPERTY()
	TScriptInterface<IHighlightableActor> CurrentHighlightedActor;

	void CursorTrace();

	UAuraAbilitySystemComponent* GetAuraASC();

	// Input handling functions
	void Move(const FInputActionValue& InputActionValue);

	void AbilityInputPressed(const FGameplayTag InputTag);

	void AbilityInputHeld(const FGameplayTag InputTag);

	void AbilityInputReleased(const FGameplayTag InputTag);
};
