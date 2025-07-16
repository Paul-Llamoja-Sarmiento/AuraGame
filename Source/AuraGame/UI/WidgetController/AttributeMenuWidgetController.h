
#pragma once

#include "CoreMinimal.h"
#include "AuraGame/UI/Widget/AttributeMenuWidgetInterface.h"
#include "Base/WidgetControllerBase.h"
#include "AttributeMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloseButtonPressed, UAttributeMenuWidgetController*, Controller);


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API UAttributeMenuWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()
public:
	// UAuraWidgetController
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams) override;

	UPROPERTY(BlueprintAssignable)
	FOnCloseButtonPressed OnAttributeMenuClosed;

protected:
	// UAuraWidgetController
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

private:
	FCloseButtonPressed OnCloseButtonPressedInternal;
	
	UFUNCTION()
	void OnMenuCloseHandle();
};
