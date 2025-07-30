
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"


/**
 * @brief Holds metadata for a single gameplay attribute.
 *
 * This struct associates a GameplayTag with display information (name, description)
 * and the corresponding FGameplayAttribute accessor. Used by UAttributeInfo to
 * drive attribute-related UI and logic in a compact, data-driven way.
 */
USTRUCT(BlueprintType)
struct FAttributeInfoStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Info")
	FGameplayTag AttributeTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	FText AttributeDescription = FText();

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Info")
	FGameplayAttribute AttributeGetter = FGameplayAttribute();
};


/**
 * @brief Data asset for organizing and managing RPG attribute metadata.
 *
 * UAttributeInfo centralizes the mapping from GameplayTags to display data
 * and attribute getters. Ideal for use with attribute menus, binding callbacks
 * or driving UI widgets without hardcoding tags or names.
 */
UCLASS()
class AURAGAME_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Looks up an attribute entry by its tag.
	 *
	 * @param AttributeTag The GameplayTag to search for.
	 * @param OutAttributeInfo Struct to populate if found.
	 * @return true if an entry with the given tag was found; false otherwise.
	 */
	bool FindAttributeInfoByTag(const FGameplayTag& AttributeTag, FAttributeInfoStruct& InAttributeInfo) const;

	// Main array containing all attribute metadata
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info", meta=(TitleProperty = "AttributeName"))
	TArray<FAttributeInfoStruct> AttributeInfoArray;

#if WITH_EDITOR
	UFUNCTION(Category = "Populate Attribute Info", CallInEditor)
	void PopulateAttributeInfo();
#endif
};
