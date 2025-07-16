
#include "AttributeMenuWidgetController.h"

#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"


void UAttributeMenuWidgetController::InitializeAuraWidgetController(const FWidgetControllerParams& InParams)
{
	checkf(InParams.Widget->GetClass()->ImplementsInterface(UAttributeMenuWidgetInterface::StaticClass()),
	       TEXT("InitializeAuraWidgetController: InParams.Widget must implement IAttributeMenuWidgetInterface"));

	Super::InitializeAuraWidgetController(InParams);

	OnCloseButtonPressedInternal.BindDynamic(this, &UAttributeMenuWidgetController::OnMenuCloseHandle);
	IAttributeMenuWidgetInterface::Execute_IBindToOnAttributeMenuCloseButtonPressed(
		ControlledWidget, OnCloseButtonPressedInternal);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
				
		}
		);
}

void UAttributeMenuWidgetController::OnMenuCloseHandle()
{
	IAttributeMenuWidgetInterface::Execute_IUnbindFromOnAttributeMenuCloseButtonPressed(ControlledWidget, OnCloseButtonPressedInternal);
	OnAttributeMenuClosed.Broadcast(this);
}
