
#pragma once

#include "CoreMinimal.h"
#include "AuraUserWidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURAGAME_API UAuraUserWidget : public UUserWidget, public IAuraUserWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void ISetWidgetController_Implementation(UObject* InWidgetController);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
};
