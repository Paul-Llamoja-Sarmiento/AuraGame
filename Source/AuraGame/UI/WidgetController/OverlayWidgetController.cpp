
#include "OverlayWidgetController.h"

#include "AuraGame/AuraGameplayTags.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"
#include "AuraGame/UI/Widget/OverlayWidgetInterface.h"


void UOverlayWidgetController::InitializeAuraWidgetController(const FWidgetControllerParams& InParams)
{
	checkf(InParams.Widget->GetClass()->ImplementsInterface(UOverlayWidgetInterface::StaticClass()),
	   TEXT("InitializeAuraWidgetController: InParams.Widget must implement IOverlayWidgetInterface"));
	
	Super::InitializeAuraWidgetController(InParams);
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Vital_Health, AuraAttributeSet->GetHealth());
	IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Vital_MaxHealth, AuraAttributeSet->GetMaxHealth());
	IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Vital_Mana, AuraAttributeSet->GetMana());
	IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Vital_MaxMana, AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Vital_Health, Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Vital_MaxHealth, Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Vital_Mana, Data.NewValue);
		}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Vital_MaxMana, Data.NewValue);
		}
		);
}
