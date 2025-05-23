// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EldenLordCharacter.generated.h"

class USphereComponent;
class UEldenCombatComponent;
class UEldenOverlay;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ELDENLORD_API AEldenLordCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEldenLordCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION()
	void OnEldenCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*
	 * Combat Interface
	 */

	virtual int32 GetPlayerLevel() override;

	/*
	 * End Combat Interface
	 */
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
protected:
	virtual void BeginPlay() override;

private:
	virtual void InitAbilityActorInfo() override;

	/*
	 * Camera
	 */
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm1;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera1;

	// UPROPERTY(EditAnywhere)
	// USphereComponent* EldenCollisionSphere;
	
	UPROPERTY(EditAnywhere)
	UEldenCombatComponent* EldenCombatComponent;


public:
	FORCEINLINE UEldenCombatComponent* GetEldenCombatComponent() const { return EldenCombatComponent; }
};
