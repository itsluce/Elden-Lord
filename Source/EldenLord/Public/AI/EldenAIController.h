// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EldenAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEldenAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTree;
};
