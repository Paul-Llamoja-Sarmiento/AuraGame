
#include "AttributeInfo.h"

#include "AuraGame/AuraGameplayTags.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"

bool UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, FAttributeInfoStruct& InAttributeInfo) const
{
	for (auto& AttributeInfo : AttributeInfoArray)
	{
		if (AttributeInfo.AttributeTag != AttributeTag) continue;
		
		InAttributeInfo = AttributeInfo;
		return true;
	}

	return false;
}

#if WITH_EDITOR
void UAttributeInfo::PopulateAttributeInfo()
{
	const FGameplayTagContainer& GameplayTagsContainer = FAttributeGameplayTags::Get().AttributeTagsContainer;	
	AttributeInfoArray.Empty();
	AttributeInfoArray.SetNumZeroed(GameplayTagsContainer.Num());

	for (int32 i = 0; i < GameplayTagsContainer.Num(); i++)
	{
		const FGameplayTag CurrentTag = GameplayTagsContainer.GetByIndex(i);
		AttributeInfoArray[i].AttributeTag = CurrentTag; // Set attribute tag

		const FGameplayTagNode* TagLastNode = UGameplayTagsManager::Get().FindTagNode(CurrentTag).Get();
		FString NodeString = FName::NameToDisplayString(TagLastNode->GetSimpleTagName().ToString(), false);
		AttributeInfoArray[i].AttributeName = FText::FromString(NodeString); // Set attribute name

		// Set attribute getter
		for (TFieldIterator<FProperty> It(UAuraAttributeSet::StaticClass()); It; ++It)
		{
			if (FGameplayAttribute::IsGameplayAttributeDataProperty(*It) &&
				FGameplayAttribute(*It).GetName() == TagLastNode->GetSimpleTagName().ToString())
			{
				AttributeInfoArray[i].AttributeGetter = FGameplayAttribute(*It);
				break;
			}
		}

		const FName TagName = CurrentTag.GetTagName();
		/* Out's */
		FString TagComment = FString();
		FName TagSource = FName();
		bool bIsTagExplicit = false;
		bool bIsRestrictedTag = false;
		bool bAllowNonRestrictedChildren = false;

		UGameplayTagsManager::Get().GetTagEditorData(
			TagName,
			TagComment,
			TagSource,
			bIsTagExplicit,
			bIsRestrictedTag,
			bAllowNonRestrictedChildren
		);

		AttributeInfoArray[i].AttributeDescription = FText::FromString(TagComment); // Set attribute description
	}
}
#endif
