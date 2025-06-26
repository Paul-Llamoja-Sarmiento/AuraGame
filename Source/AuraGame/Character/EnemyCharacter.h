
#pragma once

#include "CoreMinimal.h"
#include "AuraGame/Interaction/HighlightableActor.h"
#include "Base/AuraCharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS(Abstract)
class AURAGAME_API AEnemyCharacter : public AAuraCharacterBase, public IHighlightableActor
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	// IHighlightableActor
	virtual void IHighlight_Implementation() override;
	virtual void IUnHighlight_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	void CreateGameplayAbilitySystemObjects();
};
