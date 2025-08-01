// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class AWeapon;
class UPawnCombatComponent;

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELDENLORD_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();
	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingTarget(const FVector& Location);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetThrowAwayMontage();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetStandUpMontage();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetKnockDownMontage();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetAttackMontage();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetSummonMontage();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetCarryMontage();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FName> GetAttackMontageSection();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FName GetWeaponSocket();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<AActor> GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<AWeapon> SpawnedWeaponClass();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetImpactAngle(const FVector& ImpactPoint, AActor* Hitter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 UpdateAttackCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetHit(const FVector& ImpactPoint,AActor* Hitter);
	
};
