
#include "AttributeInfo.h"

bool UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, FAttributeInfoStruct& InAttributeInfo) const
{
	if (AttributeInfoMap.Contains(AttributeTag))
	{
		InAttributeInfo = AttributeInfoMap[AttributeTag];
		return true;
	}

	return false;
}
