
#include "EnemyWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/AuraGameplayTags.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"
#include "AuraGame/UI/Widget/EnemyWidgetInterface.h"


void UEnemyWidgetController::InitializeAuraWidgetController(const FWidgetControllerParams& InParams)
{
	checkf(InParams.Widget->GetClass()->ImplementsInterface(UEnemyWidgetInterface::StaticClass()),
	   TEXT("InitializeAuraWidgetController: InParams.Widget must implement IEnemyWidgetInterface"));
	
	Super::InitializeAuraWidgetController(InParams);
}

void UEnemyWidgetController::CleanupController()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (HealthAttributeBinding.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAuraAttributeSet::GetHealthAttribute()).Remove(HealthAttributeBinding);
		HealthAttributeBinding.Reset();
	}

	if (MaxHealthAttributeBinding.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAuraAttributeSet::GetMaxHealthAttribute()).Remove(MaxHealthAttributeBinding);
		MaxHealthAttributeBinding.Reset();
	}
}

void UEnemyWidgetController::BroadcastInitialValues()
{
	if (IsValid(ControlledWidget))
	{
		IEnemyWidgetInterface::Execute_IUpdateAttribute(ControlledWidget, Attributes_Vital_Health, AttributeSet->GetHealth());
		IEnemyWidgetInterface::Execute_IUpdateAttribute(ControlledWidget, Attributes_Secondary_MaxHealth, AttributeSet->GetMaxHealth());
	}
}

void UEnemyWidgetController::BindCallbacksToDependencies()
{
	if (!IsValid(ControlledWidget))
	{
		return;
	}

	HealthAttributeBinding = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			IEnemyWidgetInterface::Execute_IUpdateAttribute(ControlledWidget, Attributes_Vital_Health,
			                                                Data.NewValue);
		}
	);

	MaxHealthAttributeBinding = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			IEnemyWidgetInterface::Execute_IUpdateAttribute(ControlledWidget, Attributes_Secondary_MaxHealth,
			                                                Data.NewValue);
		}
	);
}
