
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Base/WidgetControllerBase.h"
#include "OverlayWidgetController.generated.h"

class UUIMessageData;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;

/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API UOverlayWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()

public:
	// UWidgetControllerBase
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams) override;
	virtual void CleanupController() override;

	void SetAttributeButtonEnabled(bool bEnabled) const;

protected:
	// UWidgetControllerBase
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI Messages")
	TMap<FGameplayTag, UUIMessageData*> UIMessagesMap;
	
private:
	FDelegateHandle HealthAttributeBinding;
	FDelegateHandle MaxHealthAttributeBinding;
	FDelegateHandle ManaAttributeBinding;
	FDelegateHandle MaxManaAttributeBinding;
	FDelegateHandle GameplayEffectBinding;

	void OnGameplayEffectAppliedToSelfHandle(const FGameplayTagContainer& TagContainer);
};
