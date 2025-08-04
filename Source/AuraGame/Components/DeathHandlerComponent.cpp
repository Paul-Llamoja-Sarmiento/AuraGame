
#include "DeathHandlerComponent.h"

#include "AuraGame/Character/Base/CharacterBase.h"
#include "Components/CapsuleComponent.h"


UDeathHandlerComponent::UDeathHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UDeathHandlerComponent::HandleDeath()
{
	if (!IsValid(OwnerCharacter) || !OwnerCharacter->HasAuthority())
	{
		return;
	}

	OwnerCharacter->SetLifeSpan(DeathLifeSpan);
	OwnerCharacter->GetWeaponMesh()->DetachFromComponent(
		FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath();
}

void UDeathHandlerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DissolveTimeline.TickTimeline(DeltaTime);
}


void UDeathHandlerComponent::MulticastHandleDeath_Implementation()
{
	ApplyRagdollPhysics();
	Dissolve();
}


void UDeathHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = CastChecked<ACharacterBase>(GetOwner());
	EnsureValidDissolveDuration();
	SetComponentTickEnabled(false);
}

void UDeathHandlerComponent::EnsureValidDissolveDuration()
{
	if (DissolveTimeInSeconds >= DeathLifeSpan)
	{
		DissolveTimeInSeconds = FMath::Max(0.0f, DeathLifeSpan - KINDA_SMALL_NUMBER);
	}
}

void UDeathHandlerComponent::ApplyRagdollPhysics() const
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	
	OwnerCharacter->GetWeaponMesh()->SetSimulatePhysics(true);
	OwnerCharacter->GetWeaponMesh()->SetEnableGravity(true);
	OwnerCharacter->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	OwnerCharacter->GetMesh()->SetSimulatePhysics(true);
	OwnerCharacter->GetMesh()->SetEnableGravity(true);
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	OwnerCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

void UDeathHandlerComponent::Dissolve()
{
	if (!IsValid(ActorDissolveMaterialInstance) || !IsValid(WeaponDissolveMaterialInstance) || !IsValid(OwnerCharacter))
	{
		return;
	}

	DissolveMaterialInstances.Empty();
	
	UMaterialInstanceDynamic* ActorDissolveMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(
		ActorDissolveMaterialInstance, OwnerCharacter);
	OwnerCharacter->GetMesh()->SetMaterial(0, ActorDissolveMaterialInstanceDynamic);
	DissolveMaterialInstances.Add(ActorDissolveMaterialInstanceDynamic);

	UMaterialInstanceDynamic* WeaponDissolveMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(
		WeaponDissolveMaterialInstance, OwnerCharacter->GetWeaponMesh());
	OwnerCharacter->GetWeaponMesh()->SetMaterial(0, WeaponDissolveMaterialInstanceDynamic);
	DissolveMaterialInstances.Add(WeaponDissolveMaterialInstanceDynamic);

	StartDissolveEffect();
}

void UDeathHandlerComponent::StartDissolveEffect()
{
	if (!IsValid(DissolveFloatCurve))
	{
		return;
	}
	
	FOnTimelineFloat ProgressBinder;
	ProgressBinder.BindDynamic(this, &UDeathHandlerComponent::DissolveProgressHandle);

	DissolveTimeline.AddInterpFloat(DissolveFloatCurve, ProgressBinder);
	DissolveTimeline.SetTimelineLength(DissolveTimeInSeconds);
	DissolveTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	SetComponentTickEnabled(true);
	DissolveTimeline.PlayFromStart();
}

void UDeathHandlerComponent::DissolveProgressHandle(float Value)
{
	for (const auto MaterialInstanceDynamic : DissolveMaterialInstances)
	{
		if (IsValid(MaterialInstanceDynamic))
		{
			MaterialInstanceDynamic->SetScalarParameterValue(DissolveParameterName, Value);
		}
	}
}
