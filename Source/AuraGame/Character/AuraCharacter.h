
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

protected:
	virtual void BeginPlay() override;	

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BaseTurnRate = 400.0f;	
};
