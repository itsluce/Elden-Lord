// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtentionComponentBase.h"
#include "PawnCombatComponent.generated.h"

/**
 * 
 */

class AWeapon;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};

UCLASS()
class ELDENLORD_API UPawnCombatComponent : public UPawnExtentionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Elden|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWeapon* InWeaponToRegister,
	                           bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Elden|Combat")
	AWeapon* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "Elden|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Elden|Combat")
	AWeapon* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Elden|Combat")
	void ToggleWeaponCollision(bool bShouldEnable,
	                           EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION(BlueprintCallable, Category = "Elden|Combat")
	void ToggleCharacterCollision(bool bShouldEnable);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	TArray<AActor*> OverlappedActors;

private:
	TMap<FGameplayTag, AWeapon*> CharacterCarriedWeaponMap;
};
