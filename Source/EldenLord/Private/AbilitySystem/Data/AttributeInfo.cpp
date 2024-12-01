// luce copyright


#include "AbilitySystem/Data/AttributeInfo.h"

FEldenAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FEldenAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute not found (%s) on Attribute Info (%s)"), *AttributeTag.ToString(),
		       *GetNameSafe(this));
	}
	return FEldenAttributeInfo();
}
