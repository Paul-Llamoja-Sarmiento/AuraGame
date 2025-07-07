// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EffectMessageWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEffectMessageWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API IEffectMessageWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IInitializeMessageWidget(const FText& NewText, UTexture2D* NewTexture);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IStartMessageAnimation();
};
