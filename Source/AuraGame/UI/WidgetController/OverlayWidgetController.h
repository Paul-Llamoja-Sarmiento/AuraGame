
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
	// UAuraWidgetController
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams) override;
	virtual void CleanupController() override;

	void SetAttributeButtonEnabled(bool bEnabled) const;

protected:
	// UAuraWidgetController
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI Messages")
	TMap<FGameplayTag, UUIMessageData*> UIMessagesMap;
	
private:
	FDelegateHandle HealthAttributeBinding;
	FDelegateHandle MaxHealthAttributeBinding;
	FDelegateHandle ManaAttributeBinding;
	FDelegateHandle MaxManaAttributeBinding;
	FDelegateHandle GameplayEffectBinding;

	void OnGameplayEffectAppliedToSelfHandle(UAbilitySystemComponent* ASC,
											 const FGameplayEffectSpec& EffectSpec,
											 FActiveGameplayEffectHandle ActiveEffectHandle);
};
