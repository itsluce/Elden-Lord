// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EldenHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayEldenWidgetController;
struct FWidgetControllerParams;
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
	UOverlayEldenWidgetController* GetOverlayEldenWidgetController(const FWidgetControllerParams WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:
	UPROPERTY()
	TObjectPtr<UEldenUserWidget>  OverlayWidget;	

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEldenUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayEldenWidgetController> OverlayEldenWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayEldenWidgetController> OverlayEldenWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

public:
	// FORCEINLINE UEldenOverlay* GetEldenOverlay() const { return EldenOverlay; }
};
