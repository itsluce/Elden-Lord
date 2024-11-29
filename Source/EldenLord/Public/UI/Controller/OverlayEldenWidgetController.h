// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/EldenWidgetController.h"
#include "OverlayEldenWidgetController.generated.h"


class UEldenUserWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UEldenUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttribbuteChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ELDENLORD_API UOverlayEldenWidgetController : public UEldenWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttribbuteChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttribbuteChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttribbuteChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttribbuteChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttribbuteChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttribbuteChangedSignature OnMaxStaminaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRow;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template <typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayEldenWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
