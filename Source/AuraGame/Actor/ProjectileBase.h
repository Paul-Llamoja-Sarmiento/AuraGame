
#pragma once

#include "CoreMinimal.h"
#include "ProjectileInterface.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;

UCLASS(Abstract)
class AURAGAME_API AProjectileBase : public AActor, public IProjectileInterface
{
	GENERATED_BODY()

public:
	AProjectileBase();

	// Projectile Interface
	virtual UProjectileMovementComponent* GetProjectileMovementComponent() const override
	{
		return ProjectileMovementComponent;
	}

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	void CreateSphereComponent();

	void CreateProjectileMovementComponent();
};
