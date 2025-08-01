
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAssetProviderInterface.generated.h"

class UCharacterAssetsInfo;
// This class does not need to be modified.
UINTERFACE()
class UCharacterAssetProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API ICharacterAssetProviderInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCharacterAssetsInfo* IGetCharacterAssetsInfo() const;
	
};
