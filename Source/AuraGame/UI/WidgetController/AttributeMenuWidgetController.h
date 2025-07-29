
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraGame/UI/Widget/AttributeMenuWidgetInterface.h"
#include "Base/WidgetControllerBase.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloseButtonPressed, UAttributeMenuWidgetController*, Controller);


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API UAttributeMenuWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()
public:
	// UWidgetControllerBase
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams) override;
	virtual void CleanupController() override;

	UPROPERTY(BlueprintAssignable)
	FOnCloseButtonPressed OnAttributeMenuClosed;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfoData;
	
	// UWidgetControllerBase
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

private:
	UPROPERTY()
	FCloseButtonPressed OnCloseButtonPressedInternal;

	TMap<FDelegateHandle, FGameplayAttribute> AttributeChangeDelegates;
	
	UFUNCTION()
	void OnMenuCloseHandle();
};
