
#pragma once

#include "CoreMinimal.h"
#include "AuraGame/Interaction/CombatInterface.h"
#include "AuraGame/Interaction/HighlightableActor.h"
#include "Base/CharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS(Abstract)
class AURAGAME_API AEnemyCharacter : public ACharacterBase, public IHighlightableActor, public ICombatInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	// IHighlightableActor
	virtual void IHighlight_Implementation() override;
	virtual void IUnHighlight_Implementation() override;

	// ICombatInterface
	virtual int32 IGetCharacterLevel_Implementation() const override { return Level; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults Combat")
	int32 Level = 1;
	
	virtual void BeginPlay() override;

private:
	void CreateGameplayAbilitySystemObjects();
};
