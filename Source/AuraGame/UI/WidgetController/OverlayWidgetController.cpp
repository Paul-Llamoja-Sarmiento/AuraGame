
#include "OverlayWidgetController.h"

#include "AuraGame/AuraGameplayTags.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"
#include "AuraGame/UI/Data/UIMessageData.h"
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
	IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Secondary_MaxHealth, AuraAttributeSet->GetMaxHealth());
	IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Vital_Mana, AuraAttributeSet->GetMana());
	IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Secondary_MaxMana, AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Vital_Health, Data.NewValue);
			}
				
		}
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Secondary_MaxHealth, Data.NewValue);
			}
		}
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Vital_Mana, Data.NewValue);
			}
		}
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget,Attributes_Secondary_MaxMana, Data.NewValue);
			}
		}
	);

	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(
		this, &UOverlayWidgetController::OnGameplayEffectAppliedToSelfHandle);
}


void UOverlayWidgetController::OnGameplayEffectAppliedToSelfHandle(UAbilitySystemComponent* ASC,
                                                                   const FGameplayEffectSpec& EffectSpec,
                                                                   FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	for (const auto& Tag : TagContainer)
	{
		if (!UIMessagesMap.Contains(Tag) || !IsValid(ControlledWidget))
		{
			return;
		}

		IOverlayWidgetInterface::Execute_IDisplayUIMessage(ControlledWidget,
		                                                   UIMessagesMap[Tag]->MessageText,
		                                                   UIMessagesMap[Tag]->MessageWidget,
		                                                   UIMessagesMap[Tag]->MessageIcon);
	}
}

