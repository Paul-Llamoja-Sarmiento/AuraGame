
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightableActor.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHighlightableActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API IHighlightableActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IHighlight();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUnHighlight();
};
