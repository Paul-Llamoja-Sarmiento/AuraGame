
#pragma once

#include "CoreMinimal.h"
#include "HUDInterface.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UUserWidgetBase;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API AHUDBase : public AHUD, public IHUDInterface
{
	GENERATED_BODY()

public:
	virtual void IInitOverlay_Implementation(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS) override;

protected:
	UPROPERTY()
	TObjectPtr<UUserWidgetBase> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& InParams);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidgetBase> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
};
