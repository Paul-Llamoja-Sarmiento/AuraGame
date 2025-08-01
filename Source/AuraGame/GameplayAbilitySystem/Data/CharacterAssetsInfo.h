
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterAssetsInfo.generated.h"


class UAnimMontage;

/**
 * 
 */
UCLASS()
class AURAGAME_API UCharacterAssetsInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Assets")
	TObjectPtr<UAnimMontage> HitReactionMontage;
};
