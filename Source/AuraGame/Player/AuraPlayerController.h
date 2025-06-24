
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


class IHighlightableActor;
class UInputAction;
class UInputMappingContext;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AuraMoveInputAction;

	UPROPERTY()
	TScriptInterface<IHighlightableActor> LastHighlightedActor;

	UPROPERTY()
	TScriptInterface<IHighlightableActor> CurrentHighlightedActor;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
};
