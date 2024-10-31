// luce copyright


#include "HUD/EldenHUD.h"
#include "UI/Widget/EldenUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "HUD/EldenOverlay.h"


void AEldenHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
