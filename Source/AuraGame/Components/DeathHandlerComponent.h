
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

protected:
	// Time in seconds before the actor is destroyed after death
	UPROPERTY(EditDefaultsOnly, Category = "DeathParameters")
	float DeathLifeSpan = 3.0f;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDeath();
};
