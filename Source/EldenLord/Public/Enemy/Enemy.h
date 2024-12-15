// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"

#include "Character/CharacterTypes.h"
#include "Interface/EnemyInterface.h"
#include "Enemy.generated.h"

class UEldenOverlay;
class UPawnSensingComponent;
class UHealthBarComponent;

UCLASS()
class ELDENLORD_API AEnemy : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	/*
	 * highlight enemy
	 */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	virtual void InitAbilityActorInfo() override;
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	
	/** </ABaseCharacter> */


	virtual int32 GetPlayerLevel() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
