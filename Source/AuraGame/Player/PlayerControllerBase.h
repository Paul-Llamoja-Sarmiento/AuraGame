
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"


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
	TScriptInterface<IHighlightableActor> LastHighlightedActor;

	UPROPERTY()
	TScriptInterface<IHighlightableActor> CurrentHighlightedActor;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	void AbilityInputPressed(FGameplayTag InputTag);

	void AbilityInputHeld(FGameplayTag InputTag);

	void AbilityInputReleased(FGameplayTag InputTag);
};
