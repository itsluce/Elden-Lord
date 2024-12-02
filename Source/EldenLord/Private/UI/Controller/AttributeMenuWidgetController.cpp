// luce copyright


#include "UI/Controller/AttributeMenuWidgetController.h"

#include "EldenGameplayTags.h"
#include "AbilitySystem/EldenAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UEldenAttributeSet* AS = CastChecked<UEldenAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	UEldenAttributeSet* AS = CastChecked<UEldenAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AS->TagToAttribute)
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FEldenAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
