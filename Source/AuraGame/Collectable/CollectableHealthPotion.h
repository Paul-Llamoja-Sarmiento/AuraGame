
#pragma once

#include "CoreMinimal.h"
#include "Base/AuraCollectableActorBase.h"
#include "CollectableHealthPotion.generated.h"

UCLASS()
class AURAGAME_API ACollectableHealthPotion : public AAuraCollectableActorBase
{
	GENERATED_BODY()

public:
	ACollectableHealthPotion();

protected:
	virtual void OnOverlapBegun(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult) override;
};
