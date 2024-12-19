// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "UI/Controller/OverlayEldenWidgetController.h"
#include "Interface/EnemyInterface.h"
#include "Enemy.generated.h"

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
	/*
	 * highlight enemy
	 */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttribbuteChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttribbuteChangedSignature OnMaxHealthChanged;

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void BeginPlay() override;

	virtual int32 GetPlayerLevel() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
