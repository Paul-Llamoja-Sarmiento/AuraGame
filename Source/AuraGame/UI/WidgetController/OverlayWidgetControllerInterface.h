
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OverlayWidgetControllerInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_DELEGATE_OneParam(FManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMaxManaChangedSignature, float, NewMaxMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

// This class does not need to be modified.
UINTERFACE()
class UOverlayWidgetControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURAGAME_API IOverlayWidgetControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IBindToOnHealthChanged(const FHealthChangedSignature& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IBindToOnMaxHealthChanged(const FMaxHealthChangedSignature& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IBindToOnManaChanged(const FManaChangedSignature& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IBindToOnMaxManaChanged(const FMaxManaChangedSignature& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUnbindFromOnHealthChanged(const FHealthChangedSignature& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUnbindFromOnMaxHealthChanged(const FMaxHealthChangedSignature& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUnbindFromOnManaChanged(const FManaChangedSignature& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IUnbindFromOnMaxManaChanged(const FMaxManaChangedSignature& Event);
};
