// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EldenAbilitySystemLibrary.generated.h"

class UCharacterClassInfo;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
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

	UFUNCTION(BlueprintCallable, Category="EldenAbilitySystemLibrary|Character Class Default")
	static void InitializeDefaultAttribute(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	                                       float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="EldenAbilitySystemLibrary|Character Class Default")
	static void GiveStartupAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
	                               ECharacterClass CharacterClass);

	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="EldenAbilitySystemLibrary|GameplayMachanics")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	                                      const TArray<AActor*>& ActorToIgnore, float Radius,
	                                      const FVector& SphereOrigin);
};
