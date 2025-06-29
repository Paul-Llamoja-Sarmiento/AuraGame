
#include "OverlayWidgetController.h"

#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"


void UOverlayWidgetController::IBindToOnHealthChanged_Implementation(const FHealthChangedSignature& Event)
{
	OnHealthChanged.Add(Event);
}

void UOverlayWidgetController::IBindToOnMaxHealthChanged_Implementation(const FMaxHealthChangedSignature& Event)
{
	OnMaxHealthChanged.Add(Event);
}

void UOverlayWidgetController::IBindToOnManaChanged_Implementation(const FManaChangedSignature& Event)
{
	OnManaChanged.Add(Event);
}

void UOverlayWidgetController::IBindToOnMaxManaChanged_Implementation(const FMaxManaChangedSignature& Event)
{
	OnMaxManaChanged.Add(Event);
}

void UOverlayWidgetController::IUnbindFromOnHealthChanged_Implementation(const FHealthChangedSignature& Event)
{
	OnHealthChanged.Remove(Event);
}

void UOverlayWidgetController::IUnbindFromOnMaxHealthChanged_Implementation(const FMaxHealthChangedSignature& Event)
{
	OnMaxHealthChanged.Remove(Event);
}

void UOverlayWidgetController::IUnbindFromOnManaChanged_Implementation(const FManaChangedSignature& Event)
{
	OnManaChanged.Remove(Event);
}

void UOverlayWidgetController::IUnbindFromOnMaxManaChanged_Implementation(const FMaxManaChangedSignature& Event)
{
	OnMaxManaChanged.Remove(Event);
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::OnHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::OnMaxHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::OnManaChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::OnMaxManaChangedCallback);
}

void UOverlayWidgetController::OnHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnManaChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxManaChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
