
#pragma once

#include "CoreMinimal.h"
#include "ProjectileInterface.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UNiagaraSystem;
class USphereComponent;

UCLASS(Abstract)
class AURAGAME_API AProjectileBase : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	AProjectileBase();

	virtual void Destroyed() override;

	// Projectile Interface
	virtual UProjectileMovementComponent* GetProjectileMovementComponent() const override
	{
		return ProjectileMovementComponent;
	}

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAudioComponent> ProjectileAudioComponent;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileLifeSpan = 10.f;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);

private:
	bool bHasHitTarget = false;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ImpactVisualEffect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ImpactSoundEffect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> LoopingSoundEffect;

	void CreateSphereComponent();

	void CreateProjectileMovementComponent();

	void ApplyImpactEffects() const;
};
