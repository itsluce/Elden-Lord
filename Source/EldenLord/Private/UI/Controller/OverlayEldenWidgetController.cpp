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
		EldenAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayEldenWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayEldenWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayEldenWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayEldenWidgetController::MaxManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		EldenAttributeSet->GetStaminaAttribute()).AddUObject(this, &UOverlayEldenWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                        EldenAttributeSet->GetMaxStaminaAttribute()).
	                        AddUObject(this, &UOverlayEldenWidgetController::MaxStaminaChanged);

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

void UOverlayEldenWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayEldenWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayEldenWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayEldenWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UOverlayEldenWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UOverlayEldenWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}
