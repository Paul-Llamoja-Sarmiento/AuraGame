
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "DeathHandlerComponent.generated.h"


class ACharacterBase;
class UCapsuleComponent;

/**
 * Component responsible for handling the death logic of the owning actor.
 * Call HandleDeath() when the owner's health reaches zero.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UDeathHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDeathHandlerComponent();

	/**
	 * Executes the death logic for the owning actor.
	 * Should be called on the Server when the owner's health reaches zero.
	 */
	void HandleDeath();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Time in seconds before the actor is destroyed after death
	UPROPERTY(EditDefaultsOnly, Category = "DeathParameters")
	float DeathLifeSpan = 4.0f;

	// Time in seconds while the actor will dissolve after death.
	// This time must be less than DeathLifeSpan.
	UPROPERTY(EditDefaultsOnly, Category = "DissolveEffect")
	float DissolveTimeInSeconds = 2.0f;

	// Name of the dissolve parameter in the material instance, used for dissolving the actor.
	UPROPERTY(EditDefaultsOnly, Category = "DissolveEffect")
	FName DissolveParameterName = "Dissolve";

	// Material instances used for the dissolve effect on the actor skeletal mesh
	UPROPERTY(EditDefaultsOnly, Category = "DissolveEffect")
	TObjectPtr<UMaterialInstance> ActorDissolveMaterialInstance;

	// Material instance used for the dissolve effect on the weapon skeletal mesh
	UPROPERTY(EditDefaultsOnly, Category = "DissolveEffect")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	// Dissolve curve used to control the dissolve effect over time
	UPROPERTY(EditDefaultsOnly, Category="DissolveEffect")
	UCurveFloat* DissolveFloatCurve;

	virtual void BeginPlay() override;

private:	
	ACharacterBase* OwnerCharacter = nullptr;

	UPROPERTY()
	FTimeline DissolveTimeline = FTimeline();

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DissolveMaterialInstances;
	
	void EnsureValidDissolveDuration();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDeath();

	void ApplyRagdollPhysics() const;
	
	void Dissolve();
	
	void StartDissolveEffect();
	
	UFUNCTION()
	void DissolveProgressHandle(float Value);
};
