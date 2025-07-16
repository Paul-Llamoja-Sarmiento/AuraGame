
#include "HUDBase.h"


#include "HeadMountedDisplayTypes.h"
#include "AuraGame/UI/Widget/Base/UserWidgetBase.h"
#include "AuraGame/UI/WidgetController/OverlayWidgetController.h"


void AHUDBase::IInitOverlay_Implementation(APlayerController* InPC, APlayerState* InPS,
	UAbilitySystemComponent* InASC, UAttributeSet* InAS)
{
	// Store init params for dynamic menus
	UIInitParams = FWidgetControllerParams(nullptr, InPC, InPS, InASC, InAS);
	
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass must be set in AAuraHUDBase!"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass must be set in AAuraHUDBase!"));
	
	UUserWidgetBase* OverlayWidget = CreateWidget<UUserWidgetBase>(GetWorld(), OverlayWidgetClass);
	const FWidgetControllerParams Params(OverlayWidget, InPC, InPS, InASC, InAS);
	OverlayWidgetController = GetOverlayWidgetController(Params);
	
	OverlayWidget->AddToViewport();
}


void AHUDBase::ShowAttributeMenu(const FVector2D& WidgetPosition)
{
	UAttributeMenuWidgetController* Controller = ShowDynamicWidget<UUserWidgetBase, UAttributeMenuWidgetController>(
		TEXT("AttributeMenu"),
		AttributeMenuWidgetClass,
		AttributeMenuWidgetControllerClass,
		WidgetPosition
	);

	if (IsValid(Controller))
	{
		Controller->OnAttributeMenuClosed.AddDynamic(this, &AHUDBase::OnAttributeMenuClosedHandle);
		OverlayWidgetController->SetAttributeButtonEnabled(false);
	}
}

void AHUDBase::HideAttributeMenu()
{
	HideDynamicWidget(TEXT("AttributeMenu"));
}

// void AHUDBase::ShowSpellMenu()
// {
// }
//
// void AHUDBase::HideSpellMenu()
// {
// }

UOverlayWidgetController* AHUDBase::GetOverlayWidgetController(const FWidgetControllerParams& InParams)
{
	if (!IsValid(OverlayWidgetController))
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->InitializeAuraWidgetController(InParams);
	}
	
	return OverlayWidgetController;
}

void AHUDBase::OnAttributeMenuClosedHandle(UAttributeMenuWidgetController* WidgetController)
{
	WidgetController->OnAttributeMenuClosed.RemoveDynamic(this, &AHUDBase::OnAttributeMenuClosedHandle);
	HideAttributeMenu();
	OverlayWidgetController->SetAttributeButtonEnabled(true);
}
