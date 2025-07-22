
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AuraGame/Interaction/CombatInterface.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateBase.generated.h"


class UAuraAttributeSet;
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
	virtual FVector GetCombatSocketLocation() const override { return FVector::ZeroVector; }

	UAuraAttributeSet* GetAttributeSet() { return AttributeSet; }

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere)
	int32 Level = 1;

private:
	void CreateGameplayAbilitySystemObjects();
};
