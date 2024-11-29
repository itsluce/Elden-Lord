// luce copyright


#include "UI/Controller/OverlayEldenWidgetController.h"

#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "AbilitySystem/EldenAttributeSet.h"

void UOverlayEldenWidgetController::BroadcastInitValues()
{
	const UEldenAttributeSet* EldenAttributeSet = CastChecked<UEldenAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(EldenAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(EldenAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(EldenAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(EldenAttributeSet->GetMaxMana());

	OnStaminaChanged.Broadcast(EldenAttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(EldenAttributeSet->GetMaxStamina());
}

void UOverlayEldenWidgetController::BindCallbacksToDependencies()
{
	const UEldenAttributeSet* EldenAttributeSet = CastChecked<UEldenAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);
		}
	);

	Cast<UEldenAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				const bool bMatchTag = Tag.MatchesTag(MessageTag);
				if (bMatchTag)
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRow.Broadcast(*Row);
				}
			}
		}
	);
}
