// luce copyright


#include "AbilitySystem/EldenAbilitySystemLibrary.h"

#include "HUD/EldenHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EldenPlayerState.h"
#include "UI/Controller/EldenWidgetController.h"

UOverlayEldenWidgetController* UEldenAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AEldenHUD* EldenHUD = Cast<AEldenHUD>(PC->GetHUD()))
		{
			AEldenPlayerState* PS = PC->GetPlayerState<AEldenPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return EldenHUD->GetOverlayEldenWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UEldenAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AEldenHUD* EldenHUD = Cast<AEldenHUD>(PC->GetHUD()))
		{
			AEldenPlayerState* PS = PC->GetPlayerState<AEldenPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return EldenHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
