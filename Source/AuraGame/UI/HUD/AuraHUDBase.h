
#pragma once

#include "CoreMinimal.h"
#include "AuraHUDInterface.h"
#include "GameFramework/HUD.h"
#include "AuraHUDBase.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API AAuraHUDBase : public AHUD, public IAuraHUDInterface
{
	GENERATED_BODY()

public:
	virtual void IInitOverlay_Implementation(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS) override;

protected:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& InParams);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
};
