// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/EldenWidgetController.h"
#include "OverlayEldenWidgetController.generated.h"


struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChangedSignature, float, NewMaxStamina);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class ELDENLORD_API UOverlayEldenWidgetController : public UEldenWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnManaChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnMaxManaChangedSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnStaminaChangedSignature OnStaminaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnMaxStaminaChangedSignature OnMaxStaminaChanged;
protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;
};
