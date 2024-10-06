// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"

#include "Character/CharacterTypes.h"
#include "Enemy.generated.h"

class UEldenOverlay;
class UPawnSensingComponent;
class UHealthBarComponent;

UCLASS()
class ELDENLORD_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	void SpawnDefaultWeapon();
	void HideHealthBar();
	void MoveToTarget();
	void InitializeEnemy();
	UPROPERTY(BlueprintReadWrite)
	class AEldenLordCharacter* EldenLordCharacter;
	// <Actor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	// </Actor>

	// <IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor*Hitter) override;
	// </IHitInterface>
protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	/** </ABaseCharacter> */
	

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(VisibleAnywhere)
	UEldenOverlay* EldenOverlay;
private:
	// AI Behavior
	void CheckCombatTarget();
	void CheckPatrolTarget();
	
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* SensingComponent;
	
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double ChasingRadius = 750.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float MinWait = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float MaxWait = 10.f;

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category= Compat)
	float PatrollingSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category= Compat)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 10.f;
	
public:
	
protected:
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	AActor* ChoosePatrolTarget();
		
	
private:
	/*
	 * AI behavior
	 */
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsInsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	/*
	 * Combat
	 */
	void StartAttackTimer();
	void ClearAttackTimer();

	

	/**
	 * Navigation
	 */
	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass1;

	FTimerHandle PatrolTimer;
	void PatrolTimeFinish();
};
