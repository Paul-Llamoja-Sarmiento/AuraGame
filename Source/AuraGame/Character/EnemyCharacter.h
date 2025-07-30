
#pragma once

#include "CoreMinimal.h"
#include "AuraGame/GameplayAbilitySystem/Data/CharacterClassInfo.h"
#include "AuraGame/Interaction/HighlightableActor.h"
#include "Base/CharacterBase.h"
#include "EnemyCharacter.generated.h"


class UEnemyWidgetController;
class UWidgetComponent;


UCLASS(Abstract)
class AURAGAME_API AEnemyCharacter : public ACharacterBase, public IHighlightableActor
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// IHighlightableActor
	virtual void IHighlight_Implementation() override;
	virtual void IUnHighlight_Implementation() override;

	// ICombatInterface
	virtual int32 IGetCharacterLevel_Implementation() const override { return Level; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults Combat")
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = "Defaults Combat")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UEnemyWidgetController> EnemyWidgetControllerClass;
	
	virtual void BeginPlay() override;

	// Character Base
	virtual void InitializeDefaultAttributes() const override;
	virtual void InitializeAbilityActorInfo() override;

	void InitializeHealthWidgetBar();

private:
	UPROPERTY()
	TObjectPtr<UEnemyWidgetController> EnemyWidgetController;
	
	void CreateGameplayAbilitySystemObjects();
};
