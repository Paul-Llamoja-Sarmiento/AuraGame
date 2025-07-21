
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AutoRun")
	float AcceptanceRadius = 50.f;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	bool bIsAutoRunning = false;
	
	FVector CachedDestination = FVector::ZeroVector;

	UPROPERTY()
	TObjectPtr<APawn> CurrentPawn = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> SplineComponent = nullptr;
};
