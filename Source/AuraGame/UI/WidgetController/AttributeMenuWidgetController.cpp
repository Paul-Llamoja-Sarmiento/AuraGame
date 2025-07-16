
#include "AttributeMenuWidgetController.h"

#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"
#include "AuraGame/GameplayAbilitySystem/Data/AttributeInfo.h"


void UAttributeMenuWidgetController::InitializeAuraWidgetController(const FWidgetControllerParams& InParams)
{
	checkf(InParams.Widget->GetClass()->ImplementsInterface(UAttributeMenuWidgetInterface::StaticClass()),
	       TEXT("InitializeAuraWidgetController: InParams.Widget must implement IAttributeMenuWidgetInterface"));

	Super::InitializeAuraWidgetController(InParams);

	OnCloseButtonPressedInternal.BindDynamic(this, &UAttributeMenuWidgetController::OnMenuCloseHandle);
	IAttributeMenuWidgetInterface::Execute_IBindToOnAttributeMenuCloseButtonPressed(
		ControlledWidget, OnCloseButtonPressedInternal);
}

void UAttributeMenuWidgetController::CleanupController()
{
	if (IsValid(ControlledWidget))
	{
		IAttributeMenuWidgetInterface::Execute_IUnbindFromOnAttributeMenuCloseButtonPressed(
			ControlledWidget, OnCloseButtonPressedInternal);
	}

	if (IsValid(AbilitySystemComponent))
	{
		for (auto& Delegate : AttributeChangeDelegates)
		{
			if (!Delegate.Key.IsValid()) continue;
			
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				Delegate.Value).Remove(Delegate.Key);
			Delegate.Key.Reset();
		}

		AttributeChangeDelegates.Empty();
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues() 
{
	check(AttributeInfoData);
	if (!IsValid(ControlledWidget) || !IsValid(AttributeSet))
	{
		return;
	}
	
	for (const auto& AttributeData: AttributeInfoData->AttributeInfoMap)
	{
		const float InitialValue = AttributeData.Value.AttributeGetter.GetNumericValue(AttributeSet);
		IAttributeMenuWidgetInterface::Execute_IUpdateAttributeValue(ControlledWidget, AttributeData.Key, AttributeData.Value, InitialValue);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfoData);
	if (!IsValid(ControlledWidget) || !IsValid(AbilitySystemComponent) || !IsValid(AttributeSet))
	{
		return;
	}

	for (const auto& AttributeData : AttributeInfoData->AttributeInfoMap)
	{
		FDelegateHandle Delegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AttributeData.Value.AttributeGetter).AddLambda(
			[this, AttributeData](const FOnAttributeChangeData& Data)
			{
				const float InitialValue = AttributeData.Value.AttributeGetter.GetNumericValue(AttributeSet);
				IAttributeMenuWidgetInterface::Execute_IUpdateAttributeValue(
					ControlledWidget, AttributeData.Key, AttributeData.Value, InitialValue);
			});

		AttributeChangeDelegates.Add(Delegate, AttributeData.Value.AttributeGetter);
	}
}

void UAttributeMenuWidgetController::OnMenuCloseHandle()
{
	IAttributeMenuWidgetInterface::Execute_IUnbindFromOnAttributeMenuCloseButtonPressed(ControlledWidget, OnCloseButtonPressedInternal);
	OnAttributeMenuClosed.Broadcast(this);
}
