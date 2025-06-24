
#pragma once

#include "CoreMinimal.h"
#include "Base/AuraCharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AURAGAME_API AEnemyCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
