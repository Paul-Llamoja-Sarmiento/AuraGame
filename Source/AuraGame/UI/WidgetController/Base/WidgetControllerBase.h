
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WidgetControllerBase.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
	{
	}

	FWidgetControllerParams(UObject* InWidget, APlayerController* InPC, APlayerState* InPS,
	                        UAbilitySystemComponent* InASC, UAttributeSet* InAS):
		Widget(InWidget),
		PlayerController(InPC),
		PlayerState(InPS),
		AbilitySystemComponent(InASC),
		AttributeSet(InAS)
	{
	} 

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> Widget;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};


/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class AURAGAME_API UWidgetControllerBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void InitializeAuraWidgetController(const FWidgetControllerParams& InParams);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UObject> ControlledWidget;

	virtual void BindCallbacksToDependencies() {}
	
	virtual void BroadcastInitialValues() {}
};
