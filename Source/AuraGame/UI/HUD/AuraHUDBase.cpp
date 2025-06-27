
#include "AuraHUDBase.h"


#include "AuraGame/UI/Widget/AuraUserWidget.h"


void AAuraHUDBase::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
