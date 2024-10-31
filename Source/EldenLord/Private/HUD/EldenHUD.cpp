// luce copyright


#include "HUD/EldenHUD.h"
#include "UI/Widget/EldenUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "HUD/EldenOverlay.h"
#include "UI/Controller/OverlayEldenWidgetController.h"


UOverlayEldenWidgetController* AEldenHUD::GetOverlayEldenWidgetController(const FWidgetControllerParams WCParams)
{
	if (OverlayEldenWidgetController == nullptr)
	{
		OverlayEldenWidgetController = NewObject<
			UOverlayEldenWidgetController>(this, OverlayEldenWidgetControllerClass);
		OverlayEldenWidgetController->SetWidgetControllerParams(WCParams);
		OverlayEldenWidgetController->BindCallbacksToDependencies();
		return OverlayEldenWidgetController;
	}
	return OverlayEldenWidgetController;
}

void AEldenHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class"));
	checkf(OverlayEldenWidgetControllerClass, TEXT("Overlay widget Controller class"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UEldenUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayEldenWidgetController* EldenWidgetController = GetOverlayEldenWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(EldenWidgetController);
	EldenWidgetController->BroadcastInitValues();
	Widget->AddToViewport();
}
