
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAttributeInfoStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Info")
	float AttributeValue = -1.f;
};

/**
 * 
 */
UCLASS()
class AURAGAME_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	bool FindAttributeInfoByTag(const FGameplayTag& AttributeTag, FAttributeInfoStruct& InAttributeInfo) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Info")
	TMap<FGameplayTag, FAttributeInfoStruct> AttributeInfoMap;
};
