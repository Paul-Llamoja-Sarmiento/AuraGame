
#include "AuraUserWidget.h"


void UAuraUserWidget::ISetWidgetController_Implementation(UObject* InWidgetController)
{
	if (!IsValid(InWidgetController))
	{
		return;
	}
	
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
