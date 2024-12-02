// luce copyright


#include "Input/EldenInputConfig.h"


const UInputAction* UEldenInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                    bool bLogNotFound) const
{
	for (const FEldenInputAction& Action : AbilityInputActions)
	{
		if (Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}
	if (!bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Input Action for InputTag [%s] on InputConfig [%s]."),
			   *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
