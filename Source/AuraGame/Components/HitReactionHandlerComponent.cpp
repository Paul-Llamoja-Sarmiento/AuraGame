
#include "HitReactionHandlerComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AuraGame/AuraGameplayTags.h"


UHitReactionHandlerComponent::UHitReactionHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHitReactionHandlerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindFromHitReactionTagEvent();
	
	Super::EndPlay(EndPlayReason);
}


void UHitReactionHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Move this to a callback for when the ASC is set, if possible.
	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(GetOwner());
	if (ASCInterface)
	{
		OwnerASC = ASCInterface->GetAbilitySystemComponent();
	}

	BindToHitReactionTagEvent();
}

void UHitReactionHandlerComponent::BindToHitReactionTagEvent()
{
	if (IsValid(OwnerASC))
	{
		HitReactionTagHandle = OwnerASC->RegisterGameplayTagEvent(Effects_HitReaction,
		                                                          EGameplayTagEventType::NewOrRemoved).
		                                 AddUObject(this, &UHitReactionHandlerComponent::OnHitReactionTagChangedHandle);
	}
}

void UHitReactionHandlerComponent::UnbindFromHitReactionTagEvent()
{
	if (IsValid(OwnerASC) && HitReactionTagHandle.IsValid())
	{
		OwnerASC->UnregisterGameplayTagEvent(HitReactionTagHandle, Effects_HitReaction,
		                                     EGameplayTagEventType::NewOrRemoved);
		HitReactionTagHandle.Reset();
	}
}

void UHitReactionHandlerComponent::OnHitReactionTagChangedHandle(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsHitReacting = NewCount > 0;
}

