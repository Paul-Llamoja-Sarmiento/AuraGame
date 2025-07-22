
#include "CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


int32 ACharacterBase::IGetCharacterLevel_Implementation() const
{
	return 1; // Default level, it can be overridden in derived classes
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesEffect);
	ApplyEffectToSelf(DefaultSecondaryAttributesEffect);
	ApplyEffectToSelf(DefaultVitalAttributesEffect);
}

void ACharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority())
	{
		return;
	}

	const auto AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!IsValid(AuraAbilitySystemComponent))
	{
		return;
	}

	AuraAbilitySystemComponent->GrantCharacterAbilities(StartupAbilities);
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
