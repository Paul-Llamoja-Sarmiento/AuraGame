
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraCollectableActorBase.generated.h"

class USphereComponent;

UCLASS(Abstract)
class AURAGAME_API AAuraCollectableActorBase : public AActor
{
	GENERATED_BODY()

public:
	AAuraCollectableActorBase();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegun(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);
};
