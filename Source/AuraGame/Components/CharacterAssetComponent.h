
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAssetComponent.generated.h"


class UCharacterAssetsInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UCharacterAssetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterAssetComponent();

	UCharacterAssetsInfo* GetCharacterAssetsInfo() const { return CharacterAssetsInfo; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Character Assets")
	TObjectPtr<UCharacterAssetsInfo> CharacterAssetsInfo;
};
