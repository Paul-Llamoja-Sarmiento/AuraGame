
#pragma once

#include "CoreMinimal.h"
#include "Base/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

UCLASS(Abstract)
class AURAGAME_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;	

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BaseTurnRate = 400.0f;

	/**
	 * This method must be called from PossessedBy (server-side) and OnRep_PlayerState (client-side).
	 * Ensures the ASC is initialized with correct owner/avatar references on both ends.
	 * Do NOT call from the constructor — ASC setup depends on replicated data (PlayerState).
	 */
	void InitializeAbilityActorInfo();

	void InitializeHUD() const;
};
