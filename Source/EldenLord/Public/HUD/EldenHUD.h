// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EldenHUD.generated.h"

class UEldenUserWidget;
class UEldenOverlay;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<UEldenUserWidget>  OverlayWidget;
	
	// UPROPERTY()
	// TObjectPtr<UEldenOverlay>  EldenOverlay;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEldenUserWidget> OverlayWidgetClass;
public:
	// FORCEINLINE UEldenOverlay* GetEldenOverlay() const { return EldenOverlay; }
};
