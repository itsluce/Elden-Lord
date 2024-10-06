// luce copyright


#include "HUD/EldenHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/EldenOverlay.h"


void AEldenHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && EldenOverlayClass)
		{
			EldenOverlay = CreateWidget<UEldenOverlay>(Controller, EldenOverlayClass);
			EldenOverlay->AddToViewport();
		}
	}
}
