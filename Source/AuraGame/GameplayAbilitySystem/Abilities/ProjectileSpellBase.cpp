
#include "ProjectileSpellBase.h"

#include "AuraGame/Actor/ProjectileBase.h"
#include "AuraGame/Interaction/CombatInterface.h"


void UProjectileSpellBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UProjectileSpellBase::SpawnProjectile(const FVector& ProjectileTargetLocation)
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
	FRotator ProjectileTargetRotation = (ProjectileTargetLocation - CombatSocketLocation).Rotation();
	ProjectileTargetRotation.Pitch = 0.f; // Ensure the projectile is horizontal to the ground
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CombatSocketLocation);
	SpawnTransform.SetRotation(ProjectileTargetRotation.Quaternion());
	
	AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
		ProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// TODO: Give the projectile a Gameplay Effect for causing damage

	Projectile->FinishSpawning(SpawnTransform);
}
