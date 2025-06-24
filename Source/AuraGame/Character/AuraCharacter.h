
#pragma once

#include "CoreMinimal.h"
#include "Base/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

UCLASS()
class AURAGAME_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
