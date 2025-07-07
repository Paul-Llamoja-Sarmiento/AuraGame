
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UIMessageData.generated.h"

class UUserWidgetBase;
/**
 * 
 */
UCLASS()
class AURAGAME_API UUIMessageData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI Messages")
	FText MessageText = FText();

	UPROPERTY(EditDefaultsOnly, Category = "UI Messages")
	TSubclassOf<UUserWidgetBase> MessageWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI Messages")
	UTexture2D* MessageIcon = nullptr;
};
