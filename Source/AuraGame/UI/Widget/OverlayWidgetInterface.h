
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OverlayWidgetInterface.generated.h"

struct FGameplayTag;

// This class does not need to be modified.
UINTERFACE()
class UOverlayWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API IOverlayWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUpdateVitalAttribute(const FGameplayTag& AttributeTag, float NewValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IDisplayItemPickedUp();
	
};
