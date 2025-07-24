
#include "ProjectileSpellBase.h"

#include "AuraGame/Actor/ProjectileBase.h"
#include "AuraGame/Interaction/CombatInterface.h"


void UProjectileSpellBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UProjectileSpellBase::SpawnProjectile() const
{
	const FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfo();
	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface)
	{
		return;
	}

	const FVector CombatSocketLocation = CombatInterface->GetCombatSocketLocation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CombatSocketLocation);
	// TODO: Set the projectile rotation

	AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// TODO: Give the projectile a Gameplay Effect for causing damage

	Projectile->FinishSpawning(SpawnTransform);
}
