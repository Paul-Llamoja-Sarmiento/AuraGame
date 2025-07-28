
#include "TargetDataUnderCursor.h"

#include "AbilitySystemComponent.h"


UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);
}


void UTargetDataUnderCursor::Activate()
{
	if (IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		ReceiveMouseCursorData();
	}
}


void UTargetDataUnderCursor::SendMouseCursorData() const
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	
	// Opens a prediction window to allow this action to be predicted on the client
	// and later confirmed or corrected by the server using the prediction key.
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	const APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!IsValid(PC))
	{
		return;
	}

	FHitResult CursorHitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	// Create a TargetData object using the hit result.
	// This wraps the hit result into a format GAS can replicate and interpret.
	FGameplayAbilityTargetData_SingleTargetHit* HitResultTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	HitResultTargetData->HitResult = CursorHitResult;

	// Package the TargetData into a handle, which can contain multiple entries
	// (though we're only sending one here).
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(HitResultTargetData);

	// Send the TargetData to the server.
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		TargetDataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// If the task is still active, and it's valid to broadcast delegates,
	// trigger the output execution pin in the Blueprint node using the associated delegate.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReady.Broadcast(TargetDataHandle);
	}
}


void UTargetDataUnderCursor::ReceiveMouseCursorData()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	
	const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

	AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(
	this, &UTargetDataUnderCursor::OnTargetDataReplicatedHandle);

	// Try to immediately execute the delegate if the data has already arrived (e.g., due to network delay).
	const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
		SpecHandle, ActivationPredictionKey);

	// If the data wasn't available yet, this marks the task as waiting.
	// GAS will now keep it alive until the client data arrives.
	if (!bCalledDelegate)
	{
		SetWaitingOnRemotePlayerData();
	}
}


void UTargetDataUnderCursor::OnTargetDataReplicatedHandle(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
                                                          FGameplayTag ActivationTag)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Consume the target data on the server so it's no longer stored or reused.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReady.Broadcast(TargetDataHandle);
	}
}
