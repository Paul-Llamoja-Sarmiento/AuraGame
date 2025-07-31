
#include "ProjectileSpellBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/AuraGameplayTags.h"
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

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(AvatarActor);
	if (!CombatInterface)
	{
		return;
	}

	const FVector CombatSocketLocation = CombatInterface->GetCombatSocketLocation();
	FRotator ProjectileTargetRotation = (ProjectileTargetLocation - AvatarActor->GetActorLocation()).Rotation();
	ProjectileTargetRotation.Pitch = 0.f; // Ensure the projectile is horizontal to the ground
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CombatSocketLocation);
	SpawnTransform.SetRotation(ProjectileTargetRotation.Quaternion());
	
	AProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
		ProjectileClass,
		SpawnTransform,
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (IsValid(SourceASC))
	{
		const float AbilityLevel = GetAbilityLevel();
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(AvatarActor);
		
		const FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(
			SpellEffectClass, AbilityLevel, EffectContextHandle);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(Damage, BaseDamage.GetValueAtLevel(AbilityLevel));
		
		Projectile->SetProjectileEffect(EffectSpecHandle);
	}
	
	Projectile->FinishSpawning(SpawnTransform);
}
