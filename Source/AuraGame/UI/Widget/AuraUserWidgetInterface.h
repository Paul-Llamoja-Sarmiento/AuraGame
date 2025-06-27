// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraUserWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAuraUserWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API IAuraUserWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ISetWidgetController(UObject* InWidgetController);
	
};
