// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterTypes.h"
#include "GameFramework/GameModeBase.h"
#include "EldenGameMode.generated.h"

class UCharacterClassInfo;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEldenGameMode();
	
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
