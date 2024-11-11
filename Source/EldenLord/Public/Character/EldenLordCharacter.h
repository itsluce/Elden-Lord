// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Item/Item.h"
#include "CharacterTypes.h"
#include "Interface/EnemyInterface.h"
#include "EldenLordCharacter.generated.h"

class UEldenOverlay;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ELDENLORD_API AEldenLordCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEldenLordCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetHUDHealth();
	virtual void Jump() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;
	/*
	 * Input Mapping
	 */
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnRight(float Value);
	void EKeyPressed();
	void Dodge();
	void RKeyPressed();

	virtual void Attack() override;
	virtual void Die() override;
	virtual bool CanAttack() override;
	void Armed();

	UFUNCTION(BlueprintCallable)
	void SetCharacterCollision(ECollisionEnabled::Type CollisionEnabled);
	/*
	 * Play Montage Function
	 */
	void PlayArmedMontage(const FName& SectionName);
	void NextAttackMontage();

	virtual void AttackEnd() override;

	UFUNCTION(BlueprintCallable)
	void DisArmEnd();

	UFUNCTION(BlueprintCallable)
	void ArmEnd();

	UFUNCTION(BlueprintCallable)
	void ArmFinish();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void NextAttackEnd();

	UFUNCTION(BlueprintCallable)
	void DrinkPotionEnd();

	UFUNCTION(BlueprintCallable)
	void DodgeDistance();

	bool CanDisArm();
	bool CanArm();

	UPROPERTY(VisibleAnywhere)
	UEldenOverlay* EldenOverlay;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> MainWeapon;

private:
	
	virtual void InitAbilityActorInfo() override;
	
	/*
	 * Camera
	 */
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm1;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera1;

	/*
	 * Overlapping Items
	 */
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	UPROPERTY(VisibleInstanceOnly)

	/*
	 * Enum State
	 */
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/*
	 * Animation montage
	 */


	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* ArmedMontage;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* DrinkMontage;

	void InitializeOverlay();
	void ResetDoublePress();
	bool HasDodgeStamina();
	bool HasAttackStamina();

	bool bIsWaitingForSecondPress;
	float FirstPressTime;
	FTimerHandle DoublePressTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DoublePressThreshold;

	FVector DodgeDirection;

	UPROPERTY(EditAnywhere, Category = "Dodge")
	float DodgeStrength = 600.f;

	bool TriggerNextAttack;
	FName AttackSectionNames[3];
	int32 CurrentComboIndex;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
