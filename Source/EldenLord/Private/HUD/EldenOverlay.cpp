// luce copyright


#include "HUD/EldenOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEldenOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UEldenOverlay::SetHealthPotionNumber(int32 HealthPotion)
{
	const FText Text = FText::FromString(FString::Printf(TEXT("%d"), HealthPotion));
	HealthPotionText->SetText(Text);
}

void UEldenOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UEldenOverlay::SetGolds(int32 Golds)
{
	if (CoinsText)
	{
		const FText Text = FText::FromString(FString::Printf(TEXT("%d"), Golds));
		CoinsText->SetText(Text);
	}
}

void UEldenOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		const FText Text = FText::FromString(FString::Printf(TEXT("%d"), Souls));
		SoulsText->SetText(Text);
	}
}
