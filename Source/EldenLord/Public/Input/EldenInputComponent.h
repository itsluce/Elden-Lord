// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "EldenInputConfig.h"
#include "EnhancedInputComponent.h"
#include "EldenInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilitiesAction(const UEldenInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                         ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);

	template<class UserObject,typename CallbackFunc>
	void BindNativeInputAction(const UEldenInputConfig* InInputConfig,const FGameplayTag& InInputTag,ETriggerEvent TriggerEvent,UserObject* ContextObject,CallbackFunc Func);

};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UEldenInputComponent::BindAbilitiesAction(const UEldenInputConfig* InputConfig, UserClass* Object,
                                               PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                               HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FEldenInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
			
		}
	}
}

template<class UserObject, typename CallbackFunc>
inline void UEldenInputComponent::BindNativeInputAction(const UEldenInputConfig* InputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	checkf(InputConfig,TEXT("Input config data asset is null,can not proceed with binding"));

	if (const UInputAction* FoundAction = InputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction,TriggerEvent,ContextObject,Func);
	}
}