
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
UCLASS()
class AURAGAME_API UOverlayWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()

public:
	// UAuraWidgetController
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams) override;

protected:
	// UAuraWidgetController
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI Messages")
	TMap<FGameplayTag, UUIMessageData*> UIMessagesMap;
	
private:
	void OnGameplayEffectAppliedToSelfHandle(UAbilitySystemComponent* ASC,
	                                         const FGameplayEffectSpec& EffectSpec,
	                                         FActiveGameplayEffectHandle ActiveEffectHandle);
};
