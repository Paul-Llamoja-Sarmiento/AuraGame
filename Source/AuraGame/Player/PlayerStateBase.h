
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AuraGame/Interaction/CombatInterface.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateBase.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API APlayerStateBase : public APlayerState, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	APlayerStateBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	// ICombatInterface
	virtual int32 IGetCharacterLevel_Implementation() const override { return Level; }

	UAttributeSet* GetAttributeSet() { return AttributeSet; }

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere)
	int32 Level = 1;

private:
	void CreateGameplayAbilitySystemObjects();
};
