// luce copyright


#include "Character/EldenLordAnimInstance.h"

#include "Character/EldenLordCharacter.h"

void UEldenLordAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EldenLordCharacter = Cast<AEldenLordCharacter>(TryGetPawnOwner());
	if (EldenLordCharacter)
	{
		CharacterMovement = EldenLordCharacter->GetCharacterMovement();
	}
}

void UEldenLordAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (CharacterMovement)
	{
	}
}
