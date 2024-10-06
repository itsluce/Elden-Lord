// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EldenHUD.generated.h"

class UEldenOverlay;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category=Elden)
	TSubclassOf<UEldenOverlay> EldenOverlayClass;

	UPROPERTY()
	UEldenOverlay* EldenOverlay;

public:
	FORCEINLINE UEldenOverlay* GetEldenOverlay() const { return EldenOverlay; }
};
