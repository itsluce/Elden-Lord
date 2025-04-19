// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "EldenAbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EldenAbilitySystemLibrary.generated.h"

class UPawnCombatComponent;
struct FGameplayEventData;
class UCharacterClassInfo;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class UOverlayEldenWidgetController;
/**
 * 
 */
UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EWarriorValidType : uint8
{
	Valid,
	Invalid
};

UCLASS()
class ELDENLORD_API UEldenAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UEldenAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);
	
	static UEldenAbilitySystemComponent* NativeGetEldenASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "EldenAbilitySystemLibrary|GameplayMechanics")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "EldenAbilitySystemLibrary|GameplayMechanics")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor,FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility", meta = (DisplayName = "Get ImpactPoint from EventData"))
	static bool GetImpactPointFromEventData(const FGameplayEventData& EventData, FVector& OutImpactPoint);

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

	UFUNCTION(BlueprintPure, Category = "EldenAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category = "EldenAbilitySystemLibrary|GameplayMechanics")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintPure, Category = "EldenAbilitySystemLibrary|GameplayMechanics")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "EldenAbilitySystemLibrary|GameplayMechanics",
		meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	UFUNCTION(BlueprintCallable,Category = "Warrior|FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator,AActor* InTargetActor,const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "EldenAbilitySystemLibrary|GameplayMechanics", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck,EWarriorConfirmType& OutConfirmType);
};
