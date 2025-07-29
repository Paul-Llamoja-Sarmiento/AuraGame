
#include "CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/AuraGame.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
