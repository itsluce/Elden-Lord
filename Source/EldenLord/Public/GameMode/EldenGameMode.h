// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterTypes.h"
#include "GameFramework/GameModeBase.h"
#include "EldenGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AEldenGameMode();
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
};
