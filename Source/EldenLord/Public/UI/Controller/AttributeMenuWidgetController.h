// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/Controller/EldenWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


struct FEldenAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FEldenAttributeInfo&, Info);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ELDENLORD_API UAttributeMenuWidgetController : public UEldenWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitValues() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& Attribute) const;
};
