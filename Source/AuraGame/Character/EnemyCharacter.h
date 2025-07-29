
#pragma once

#include "CoreMinimal.h"
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
	
	// IHighlightableActor
	virtual void IHighlight_Implementation() override;
	virtual void IUnHighlight_Implementation() override;

	// ICombatInterface
	virtual int32 IGetCharacterLevel_Implementation() const override { return Level; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults Combat")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UEnemyWidgetController> EnemyWidgetControllerClass;
	
	virtual void BeginPlay() override;

	virtual void InitializeAbilityActorInfo() override;

	void InitializeHealthWidgetBar();

private:
	UPROPERTY()
	TObjectPtr<UEnemyWidgetController> EnemyWidgetController;
	
	void CreateGameplayAbilitySystemObjects();
};
