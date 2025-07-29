// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "EnemyWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEnemyWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API IEnemyWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUpdateAttribute(const FGameplayTag& AttributeTag, float NewValue);
};
