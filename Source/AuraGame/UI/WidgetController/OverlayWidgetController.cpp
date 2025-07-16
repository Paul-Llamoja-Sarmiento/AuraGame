
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

void UOverlayWidgetController::CleanupController()
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

	if (ManaAttributeBinding.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAuraAttributeSet::GetManaAttribute()).Remove(ManaAttributeBinding);
		ManaAttributeBinding.Reset();
	}

	if (MaxManaAttributeBinding.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UAuraAttributeSet::GetMaxManaAttribute()).Remove(MaxManaAttributeBinding);
		MaxManaAttributeBinding.Reset();
	}

	if (GameplayEffectBinding.IsValid())
	{
		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.Remove(GameplayEffectBinding);
		GameplayEffectBinding.Reset();
	}
}

void UOverlayWidgetController::SetAttributeButtonEnabled(bool bEnabled) const
{
	if (IsValid(ControlledWidget))
	{
		IOverlayWidgetInterface::Execute_IToggleAttributeMenuButton(ControlledWidget, bEnabled);
	}
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

	HealthAttributeBinding = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Vital_Health,
				                                                       Data.NewValue);
			}
		}
	);

	MaxHealthAttributeBinding = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Secondary_MaxHealth,
				                                                       Data.NewValue);
			}
		}
	);

	ManaAttributeBinding = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Vital_Mana,
				                                                       Data.NewValue);
			}
		}
	);

	MaxManaAttributeBinding = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (IsValid(ControlledWidget))
			{
				IOverlayWidgetInterface::Execute_IUpdateVitalAttribute(ControlledWidget, Attributes_Secondary_MaxMana,
				                                                       Data.NewValue);
			}
		}
	);

	GameplayEffectBinding = AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(
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

