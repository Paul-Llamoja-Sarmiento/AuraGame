
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightingComponent.generated.h"


class IHighlightableActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UHighlightingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHighlightingComponent();

	bool HasCurrentHighlightedActor() const { return CurrentHighlightedActor != nullptr; }
	
	UPROPERTY(EditDefaultsOnly, Category = "Highlighting")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	TScriptInterface<IHighlightableActor> LastHighlightedActor = nullptr;

	UPROPERTY()
	TScriptInterface<IHighlightableActor> CurrentHighlightedActor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CachedPlayerController;
	
	void PerformCursorTrace();
};
