// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "EldenController.generated.h"

class UEldenAbilitySystemComponent;
class UEldenInputConfig;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AEldenController();
	virtual void Tick(float DeltaSeconds) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> EldenContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAxis;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> TurnAxis;

	UPROPERTY()
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Input_PickUpStonesAbility(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetComplete(const FInputActionValue& InputActionValue);

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;


	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY
	(EditDefaultsOnly, Category="Input")
	TObjectPtr<UEldenInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UEldenAbilitySystemComponent> EldenAbilitySystemComponent;

	UEldenAbilitySystemComponent* GetASC();

	FGenericTeamId HeroTeamID;
};
