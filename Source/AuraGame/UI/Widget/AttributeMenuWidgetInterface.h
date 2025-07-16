
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeMenuWidgetInterface.generated.h"


struct FGameplayTag;
struct FAttributeInfoStruct;


DECLARE_DYNAMIC_DELEGATE(FCloseButtonPressed);


// This class does not need to be modified.
UINTERFACE()
class UAttributeMenuWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API IAttributeMenuWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IBindToOnAttributeMenuCloseButtonPressed(const FCloseButtonPressed& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUnbindFromOnAttributeMenuCloseButtonPressed(const FCloseButtonPressed& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUpdateAttributeValue(const FGameplayTag& GameplayTag, const FAttributeInfoStruct& AttributeInfo, float NewValue);
};
