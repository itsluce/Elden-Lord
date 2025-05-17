// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EldenLordCharacter.generated.h"

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

	UPROPERTY(EditAnywhere)
	UEldenCombatComponent* EldenCombatComponent;


public:
	FORCEINLINE UEldenCombatComponent* GetEldenCombatComponent() const { return EldenCombatComponent; }
};
