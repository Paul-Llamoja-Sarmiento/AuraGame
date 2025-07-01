
#include "GameplayEffectApplierComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


UGameplayEffectApplierComponent::UGameplayEffectApplierComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGameplayEffectApplierComponent::OnBeginOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                           bool bFromSweep, const FHitResult& SweepResult)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!IsValid(TargetASC))
	{
		return;
	}
	
	for (const auto& EffectConfig : EffectsToApply)
	{
		if (EffectConfig.ApplicationPolicy == EEffectApplicationPolicy::DoNotApply)
		{
			continue;
		}

		if (EffectConfig.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetASC, EffectConfig);
		}
	}

	if (bShouldDestroyAfterApplyEffects)
	{
		GetOwner()->Destroy();
	}
}

void UGameplayEffectApplierComponent::OnEndOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!IsValid(TargetASC))
	{
		return;
	}

	for (const auto& EffectConfig : EffectsToApply)
	{
		if (EffectConfig.ApplicationPolicy == EEffectApplicationPolicy::DoNotApply)
		{
			continue;
		}

		if (EffectConfig.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetASC, EffectConfig);
		}

		if (EffectConfig.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			RemoveEffectsFromTarget(TargetASC);
		}
	}

	if (bShouldDestroyAfterApplyEffects)
	{
		GetOwner()->Destroy();
	}
}

void UGameplayEffectApplierComponent::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* CollisionVolume = GetOwner()->FindComponentByTag<UPrimitiveComponent>(CollisionVolumeTag);
	check(CollisionVolume);
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &UGameplayEffectApplierComponent::OnBeginOverlapHandle);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &UGameplayEffectApplierComponent::OnEndOverlapHandle);
}

void UGameplayEffectApplierComponent::ApplyEffectToTarget(UAbilitySystemComponent* TargetASC, const FEffectApplicationConfig& EffectConfig)
{
	check(EffectConfig.GameplayEffect);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		EffectConfig.GameplayEffect, 1.0f, EffectContextHandle);

	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data);

	const bool bHasInfiniteDuration = EffectSpecHandle.Data->Def->DurationPolicy ==
		EGameplayEffectDurationType::Infinite;
	if (bHasInfiniteDuration && EffectConfig.RemovalPolicy != EEffectRemovalPolicy::DoNotRemove)
	{
		ActiveEffectsMap.FindOrAdd(TargetASC).Add(ActiveEffectHandle);
	}
}

void UGameplayEffectApplierComponent::RemoveEffectsFromTarget(UAbilitySystemComponent* TargetASC)
{
	if (!ActiveEffectsMap.Contains(TargetASC))
	{
		return;
	}

	for (const auto& ActiveEffectHandle : ActiveEffectsMap[TargetASC])
	{
		if (ActiveEffectHandle.IsValid())
		{
			TargetASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
		}
	}

	ActiveEffectsMap.Remove(TargetASC);
}

