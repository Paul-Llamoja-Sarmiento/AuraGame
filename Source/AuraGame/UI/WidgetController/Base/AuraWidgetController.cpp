
#include "AuraWidgetController.h"


void UAuraWidgetController::InitializeAuraWidgetController(const FWidgetControllerParams& InParams)
{
	PlayerController = InParams.PlayerController;
	PlayerState = InParams.PlayerState;
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;
	ControlledWidget = InParams.Widget;

	BindCallbacksToDependencies();
	BroadcastInitialValues();
}
