// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EldenGameplayAbility.h"
#include "EldenTargetLock.generated.h"

class UEldenUserWidget;
/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenTargetLock : public UEldenGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

private:
	void TryLockOnTarget();
	void GetAvailableTargetToLock();
	AActor* GetNearestAvailableTargetToLock(const TArray<AActor*>& FoundTargets);
	void DrawTargetToLockWidget();
	void SetTargetToLockWidgetPosition();

	UFUNCTION(BlueprintCallable)
	void OnLockTargetTick(float DeltaTime);
	
	void CancelTargetToLockAbility();
	void CleanUp();

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float BoxTraceDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	bool bShowDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	TSubclassOf<UEldenUserWidget> TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	float TargetInterpSpeed = 5.f;
	
	UPROPERTY()
	TArray<AActor*> AvailableTargetActorsToLock;

	UPROPERTY()
	AActor* CurrentActorToLock;
	
	UPROPERTY()
	UEldenUserWidget* WidgetToDraw;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;
};
