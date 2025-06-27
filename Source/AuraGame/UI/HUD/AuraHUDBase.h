
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUDBase.generated.h"


class UAuraUserWidget;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API AAuraHUDBase : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
};
