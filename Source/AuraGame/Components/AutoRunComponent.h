
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoRunComponent.generated.h"


class UNavigationSystemV1;
class USplineComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UAutoRunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAutoRunComponent();

	void TryAutoRunToCursorLocation(const APlayerController* PlayerController);
	
	void StopAutoRun();
	
	bool IsAutoRunning() const { return bIsAutoRunning; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AutoRun|Navigation")
	float AcceptanceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, Category="AutoRun|Movement")
	FVector NavQueryExtent = FVector(400.f, 400.f, 250.f);
	
	virtual void BeginPlay() override;

private:
	bool bIsAutoRunning = false;
	
	FVector CachedDestination = FVector::ZeroVector;

	UPROPERTY()
	TObjectPtr<APawn> CurrentPawn = nullptr;
	
	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent = nullptr;
};
