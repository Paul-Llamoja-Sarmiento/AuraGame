
#pragma once

#include "CoreMinimal.h"
#include "Base/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class USplineComponent;
class UAutoRunComponent;

UCLASS(Abstract)
class AURAGAME_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	/* ICombatInterface */
	
	virtual int32 IGetCharacterLevel_Implementation() const override;
	virtual void Die() override { }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	virtual void BeginPlay() override;

	/* CharacterBase */
	
	virtual void InitializeDefaultAttributes() const;

	/**
	 * This method must be called from PossessedBy (server-side) and OnRep_PlayerState (client-side).
	 * Ensures the ASC is initialized with correct owner/avatar references on both ends.
	 * Do NOT call from the constructor — ASC setup depends on replicated data (PlayerState).
	 */
	virtual void InitializeAbilityActorInfo() override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BaseTurnRate = 400.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAutoRunComponent> AutoRunComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

	void InitializeHUD() const;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float InLevel = 1.0f) const;

	void AddCharacterAbilities() const;
};
