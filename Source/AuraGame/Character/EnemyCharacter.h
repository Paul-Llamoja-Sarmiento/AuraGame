
#pragma once

#include "CoreMinimal.h"
#include "Base/AuraCharacterBase.h"
#include "EnemyCharacter.generated.h"

UCLASS(Abstract)
class AURAGAME_API AEnemyCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
};
