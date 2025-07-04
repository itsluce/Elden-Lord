// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "UI/Controller/OverlayEldenWidgetController.h"
#include "Interface/EnemyInterface.h"
#include "Enemy.generated.h"

class UEnemyCombatComponent;
class AEldenAIController;
class UBehaviorTree;
class UWidgetComponent;
class UEldenOverlay;
class UPawnSensingComponent;
class UHealthBarComponent;

UCLASS()
class ELDENLORD_API AEnemy : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void PossessedBy(AController* NewController) override;
	/*
	 * highlight enemy
	 */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttribbuteChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttribbuteChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void Die() override;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReact = false;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void BeginPlay() override;
	virtual void InitializeDefaultAttribute() const override;

	virtual int32 GetPlayerLevel() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEldenAIController> EldenAIController;
	
	UPROPERTY(EditAnywhere)
	UEnemyCombatComponent* EnemyCombatComponent;
public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
