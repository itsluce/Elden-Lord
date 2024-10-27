// luce copyright


#include "UI/Widget/EldenUserWidget.h"

void UEldenUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
