// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EldenAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class UOverlayEldenWidgetController;
/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="EldenAbilitySystemLibrary|WidgetController")
	static UOverlayEldenWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="EldenAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
};
