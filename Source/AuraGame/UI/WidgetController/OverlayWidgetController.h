
#pragma once

#include "CoreMinimal.h"
#include "OverlayWidgetControllerInterface.h"
#include "Base/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


struct FOnAttributeChangeData;


/**
 * 
 */
UCLASS()
class AURAGAME_API UOverlayWidgetController : public UAuraWidgetController, public IOverlayWidgetControllerInterface
{
	GENERATED_BODY()

public:
	// IOverlayWidgetControllerInterface
	virtual void IBindToOnHealthChanged_Implementation(const FHealthChangedSignature& Event) override;
	virtual void IBindToOnMaxHealthChanged_Implementation(const FMaxHealthChangedSignature& Event) override;
	virtual void IBindToOnManaChanged_Implementation(const FManaChangedSignature& Event) override;
	virtual void IBindToOnMaxManaChanged_Implementation(const FMaxManaChangedSignature& Event) override;
	virtual void IUnbindFromOnHealthChanged_Implementation(const FHealthChangedSignature& Event) override;
	virtual void IUnbindFromOnMaxHealthChanged_Implementation(const FMaxHealthChangedSignature& Event) override;
	virtual void IUnbindFromOnManaChanged_Implementation(const FManaChangedSignature& Event) override;
	virtual void IUnbindFromOnMaxManaChanged_Implementation(const FMaxManaChangedSignature& Event) override;

	// UAuraWidgetController
	virtual void BroadcastInitialValues() override;

protected:
	UPROPERTY()
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY()
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY()
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY()
	FOnMaxManaChangedSignature OnMaxManaChanged;

	virtual void BindCallbacksToDependencies() override;

private:
	void OnHealthChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnManaChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnMaxManaChangedCallback(const FOnAttributeChangeData& Data) const;
};
