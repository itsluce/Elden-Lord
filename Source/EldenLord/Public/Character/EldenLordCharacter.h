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
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/*
	 * Combat Interface
	 */
	
	virtual int32 GetPlayerLevel() override;
	
	/*
	 * End Combat Interface
	 */
	
protected:
	virtual void BeginPlay() override;
	/*
	 * Input Mapping
	 */
	
	/*
	 * Play Montage Function
	 */
	
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

	/*
	 * Enum State
	 */

	/*
	 * Animation montage
	 */

public:
	
};
