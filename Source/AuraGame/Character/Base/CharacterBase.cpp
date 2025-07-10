
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

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesEffect);
	ApplyEffectToSelf(DefaultSecondaryAttributesEffect);
}

void ACharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float InLevel) const
{
	check(AbilitySystemComponent);
	check(DefaultPrimaryAttributesEffect);

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass, InLevel, EffectContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}
