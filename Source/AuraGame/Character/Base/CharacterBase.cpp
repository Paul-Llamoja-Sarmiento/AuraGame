
#include "CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/AuraGame.h"
#include "AuraGame/Components/HitReactionHandlerComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitReactionHandlerComponent = CreateDefaultSubobject<UHitReactionHandlerComponent>(TEXT("HitReactionHandler"));
}


int32 ACharacterBase::IGetCharacterLevel_Implementation() const
{
	return 1; // Default level, it can be overridden in derived classes
}

FVector ACharacterBase::GetCombatSocketLocation() const
{
	check(WeaponMesh);
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}

