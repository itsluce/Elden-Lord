// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EldenOverlay.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetHealthBarPercent(float Percent);
	void SetHealthPotionNumber(int32 HealthPotion);
	void SetStaminaBarPercent(float Percent);
	void SetGolds(int32 Golds);
	void SetSouls(int32 Souls);
	
private:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* HealthPotionText;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CoinsText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SoulsText;
public:
	FORCEINLINE UTextBlock* GetHealthPotionText() {return HealthPotionText;}
};