// luce copyright


#include "UI/Controller/AttributeMenuWidgetController.h"

#include "EldenGameplayTags.h"
#include "AbilitySystem/EldenAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	UEldenAttributeSet* AS = CastChecked<UEldenAttributeSet>(AttributeSet);

	check(AttributeInfo);

	FEldenAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(
		FEldenGameplayTags::Get().Attributes_Primary_Mind);
	Info.AttributeValue = AS->GetMind();
	AttributeInfoDelegate.Broadcast(Info);
}
