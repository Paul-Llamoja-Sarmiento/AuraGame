
#pragma once

#include "CoreMinimal.h"
#include "Base/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


/**
 * 
 */
UCLASS()
class AURAGAME_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	// UAuraWidgetController
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams) override;

protected:
	// UAuraWidgetController
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
