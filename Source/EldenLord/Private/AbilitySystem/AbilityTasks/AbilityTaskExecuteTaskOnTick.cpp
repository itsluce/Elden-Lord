// luce copyright


#include "AbilitySystem/AbilityTasks/AbilityTaskExecuteTaskOnTick.h"

UAbilityTaskExecuteTaskOnTick::UAbilityTaskExecuteTaskOnTick()
{
	bTickingTask = true;
}

UAbilityTaskExecuteTaskOnTick* UAbilityTaskExecuteTaskOnTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	UAbilityTaskExecuteTaskOnTick* Node = NewAbilityTask<UAbilityTaskExecuteTaskOnTick>(OwningAbility);
	return  Node;
}

void UAbilityTaskExecuteTaskOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}
