
#pragma once

#include "CoreMinimal.h"
#include "Base/WidgetControllerBase.h"
#include "EnemyWidgetController.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API UEnemyWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()

public:
	// UWidgetControllerBase
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams) override;
	virtual void CleanupController() override;


protected:
	// UWidgetControllerBase
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

private:
	FDelegateHandle HealthAttributeBinding;
	FDelegateHandle MaxHealthAttributeBinding;
};
