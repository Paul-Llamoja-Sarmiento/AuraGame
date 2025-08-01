
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AuraGame/Interaction/CombatInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"


class UHitReactionHandlerComponent;
class UGameplayAbility;
class UAuraAttributeSet;
class UGameplayEffect;
class UAbilitySystemComponent;


UCLASS(Abstract)
class AURAGAME_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/* IAbilitySystemInterface */
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	UAuraAttributeSet* GetAttributeSet() { return AttributeSet; }

	/* ICombatInterface */
	
	virtual int32 IGetCharacterLevel_Implementation() const override;
	virtual FVector GetCombatSocketLocation() const override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHitReactionHandlerComponent> HitReactionHandlerComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	FName WeaponSocketName = "weapon-socket";

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffect;

	virtual void InitializeDefaultAttributes() const {}

	virtual void InitializeAbilityActorInfo() {}
};
