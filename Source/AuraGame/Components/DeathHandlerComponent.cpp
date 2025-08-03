
#include "DeathHandlerComponent.h"

#include "AuraGame/Character/Base/CharacterBase.h"
#include "Components/CapsuleComponent.h"


UDeathHandlerComponent::UDeathHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UDeathHandlerComponent::HandleDeath()
{
	ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (!IsValid(OwnerCharacter) || !OwnerCharacter->HasAuthority())
	{
		return;
	}

	OwnerCharacter->SetLifeSpan(DeathLifeSpan);
	OwnerCharacter->GetWeaponMesh()->DetachFromComponent(
		FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MulticastHandleDeath();
}

void UDeathHandlerComponent::MulticastHandleDeath_Implementation()
{
	const ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(GetOwner());
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
