
#pragma once

#include "CoreMinimal.h"
#include "HUDInterface.h"
#include "AuraGame/UI/Widget/Base/UserWidgetBase.h"
#include "AuraGame/UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraGame/UI/WidgetController/Base/WidgetControllerBase.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;


struct FUIInitParams
{
	APlayerController* PC;
	APlayerState* PS;
	UAbilitySystemComponent* ASC;
	UAttributeSet* AS;

	FUIInitParams() : PC(nullptr), PS(nullptr), ASC(nullptr), AS(nullptr) {}
	FUIInitParams(APlayerController* InPC, APlayerState* InPS,
				  UAbilitySystemComponent* InASC, UAttributeSet* InAS)
		: PC(InPC), PS(InPS), ASC(InASC), AS(InAS) {}
};


/**
 * 
 */
UCLASS(Abstract)
class AURAGAME_API AHUDBase : public AHUD, public IHUDInterface
{
	GENERATED_BODY()

public:
	virtual void IInitOverlay_Implementation(APlayerController* InPC, APlayerState* InPS, UAbilitySystemComponent* InASC, UAttributeSet* InAS) override;

	// Show/hide dynamic menus
	UFUNCTION(BlueprintCallable, Category = "UI Menu")
	void ShowAttributeMenu(const FVector2D& WidgetPosition = FVector2D(0.f, 0.f));

	UFUNCTION(BlueprintCallable, Category = "UI Menu")
	void HideAttributeMenu();

	// UFUNCTION(BlueprintCallable, Category = "UI Menu")
	// void ShowSpellMenu();
	//
	// UFUNCTION(BlueprintCallable, Category = "UI Menu")
	// void HideSpellMenu();

protected:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& InParams);

	// Generic factory for dynamic widgets
	template<typename WidgetType, typename ControllerType>
	ControllerType* ShowDynamicWidget(const FName& WidgetName,
									  TSubclassOf<WidgetType> WidgetClass,
									  TSubclassOf<ControllerType> ControllerClass,
									  const FVector2D& WidgetPosition = FVector2D(0.f, 0.f))
	{
		if (UWidgetControllerBase** FoundController = DynamicControllers.Find(WidgetName))
		{
			return Cast<ControllerType>(*FoundController);
		}

		// Create Widget
		WidgetType* CurrentWidget = CreateWidget<WidgetType>(GetWorld(), WidgetClass);
		if (!CurrentWidget)
		{
			return nullptr;
		}

		// Init controller
		ControllerType* Controller = NewObject<ControllerType>(this, ControllerClass);
		UIInitParams.Widget = CurrentWidget;
		Controller->InitializeAuraWidgetController(UIInitParams);

		// Store
		DynamicControllers.Add(WidgetName, Controller);
		DynamicWidgets.Add(WidgetName, CurrentWidget);

		// Show
		CurrentWidget->SetPositionInViewport(WidgetPosition);
		CurrentWidget->AddToViewport();
		return Controller;
	}

	void HideDynamicWidget(const FName& WidgetName)
	{
		if (UWidgetControllerBase** FoundController = DynamicControllers.Find(WidgetName))
		{
			(*FoundController)->CleanupController();
		}
		
		if (UUserWidgetBase** FoundWidget = DynamicWidgets.Find(WidgetName))
		{
			if (UUserWidgetBase* Widget = *FoundWidget)
			{
				Widget->RemoveFromParent();
			}
			DynamicWidgets.Remove(WidgetName);
		}
		DynamicControllers.Remove(WidgetName);
	}
	
private:
	// Blueprint config
	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UUserWidgetBase> OverlayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UUserWidgetBase> AttributeMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	// UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	// TSubclassOf<UUserWidget> SpellMenuWidgetClass;
	//
	// UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	// TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	// Maps for dynamic UI
	UPROPERTY()
	TMap<FName, UUserWidgetBase*> DynamicWidgets;

	UPROPERTY()
	TMap<FName, UWidgetControllerBase*> DynamicControllers;

	// Stored init params
	FWidgetControllerParams UIInitParams;

	UFUNCTION()
	void OnAttributeMenuClosedHandle(UAttributeMenuWidgetController* WidgetController);
};
