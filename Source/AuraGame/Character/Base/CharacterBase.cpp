
#include "CharacterBase.h"

#include "AbilitySystemComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitializeDefaultPrimaryAttributes()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}
	
	check(DefaultPrimaryAttributesEffect);

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultPrimaryAttributesEffect, 1.0f, EffectContextHandle);

	FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data);
}
