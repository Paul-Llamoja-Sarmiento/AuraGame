
#include "AuraHUDBase.h"


#include "AuraGame/UI/Widget/Base/AuraUserWidget.h"
#include "AuraGame/UI/WidgetController/OverlayWidgetController.h"
#include "Kismet/KismetSystemLibrary.h"


void AAuraHUDBase::IInitOverlay_Implementation(APlayerController* InPC, APlayerState* InPS,
	UAbilitySystemComponent* InASC, UAttributeSet* InAS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass must be set in AAuraHUDBase!"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass must be set in AAuraHUDBase!"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	if (!UKismetSystemLibrary::DoesImplementInterface(Widget, UAuraUserWidgetInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("OverlayWidgetClass %s does not) implement IAuraUserWidgetInterface!"), *OverlayWidgetClass->GetName());
		return;
	}
	
	const FWidgetControllerParams Params(InPC, InPS, InASC, InAS);
	OverlayWidgetController = GetOverlayWidgetController(Params);
	IAuraUserWidgetInterface::Execute_ISetWidgetController(Widget, OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

UOverlayWidgetController* AAuraHUDBase::GetOverlayWidgetController(const FWidgetControllerParams& InParams)
{
	if (!IsValid(OverlayWidgetController))
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->InitializeAuraWidgetController(InParams);
	}
	
	return OverlayWidgetController;
}
