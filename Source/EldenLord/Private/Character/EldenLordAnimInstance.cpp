// luce copyright


#include "Character/EldenLordAnimInstance.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
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

bool UEldenLordAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UEldenAbilitySystemLibrary::NativeDoesActorHaveTag(OwningPawn,TagToCheck);
	}

	return false;
}
