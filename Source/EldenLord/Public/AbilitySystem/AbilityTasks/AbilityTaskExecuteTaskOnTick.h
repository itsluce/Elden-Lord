// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTaskExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegete,float, DeltaTime);
/**
 * 
 */
UCLASS()
class ELDENLORD_API UAbilityTaskExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTaskExecuteTaskOnTick();
	UFUNCTION(BlueprintCallable, Category = "Elden|AbilityTask",
		meta=(HidePin= "OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly=true))
	static UAbilityTaskExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegete OnAbilityTaskTick;
};
