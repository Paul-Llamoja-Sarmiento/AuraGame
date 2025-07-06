
#include "WidgetControllerBase.h"


void UWidgetControllerBase::InitializeAuraWidgetController(const FWidgetControllerParams& InParams)
{
	PlayerController = InParams.PlayerController;
	PlayerState = InParams.PlayerState;
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;
	ControlledWidget = InParams.Widget;

	BindCallbacksToDependencies();
	BroadcastInitialValues();
}
