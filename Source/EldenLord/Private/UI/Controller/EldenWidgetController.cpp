// luce copyright


#include "UI/Controller/EldenWidgetController.h"

void UEldenWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AttributeSet = WCParams.AttributeSet;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
}

void UEldenWidgetController::BroadcastInitValues()
{
}

void UEldenWidgetController::BindCallbacksToDependencies()
{
}
