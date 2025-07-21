
#include "AutoRunComponent.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AuraGame/AuraGame.h"
#include "Components/SplineComponent.h"


UAutoRunComponent::UAutoRunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAutoRunComponent::TryAutoRunToCursorLocation(const APlayerController* PlayerController)
{
	auto NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!IsValid(NavSystem) || !IsValid(PlayerController) || !IsValid(SplineComponent) || !IsValid(CurrentPawn))
	{
		return;
	}
	
	FHitResult NavChannelCursorHitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Navigation, false, NavChannelCursorHitResult);
	if (!NavChannelCursorHitResult.bBlockingHit)
	{
		return;
	}
	
	// Projecting a point from the cursor impact point to the NavMesh with a larger-than-default Query Extent,
	// so there are better chances to reach for the NavMesh and return a point.
	// Then, generate a path from the pawn location to this point (only if found).
	// NOTE: Default QueryExtend = FVector(50.0f, 50.0f, 250.0f), but we will be using a larger one to ensure
	// that we can find a point on the NavMesh even if the cursor is far away from it. This is suitable for
	// our level design, but it may be adjusted different if needed
	FNavLocation ImpactPointNavLocation;
	const FVector QueryingExtent = NavQueryExtent;
	const FNavAgentProperties& NavAgentProps = PlayerController->GetNavAgentPropertiesRef();
	const bool bNavLocationFound = NavSystem->ProjectPointToNavigation(NavChannelCursorHitResult.ImpactPoint,
																	   ImpactPointNavLocation, QueryingExtent,
																	   &NavAgentProps);
	if (!bNavLocationFound)
	{
		return;
	}
	
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		GetWorld(), CurrentPawn->GetActorLocation(), ImpactPointNavLocation.Location);

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		SplineComponent->ClearSplinePoints();
		for (const FVector& PointLocation : NavPath->PathPoints)
		{
			SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
			DrawDebugSphere(GetWorld(), PointLocation, 10.f, 12, FColor::Red, false, 5.f);
		}

		CachedDestination = NavPath->PathPoints.Last();
		bIsAutoRunning = true;
	}		
}

void UAutoRunComponent::StopAutoRun()
{
	if (bIsAutoRunning)
	{
		bIsAutoRunning = false;
	}
}

void UAutoRunComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentPawn = Cast<APawn>(GetOwner());
	if (!IsValid(CurrentPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("AutoRunComponent: Owner isn’t a Pawn! or is nullptr"));
		return;
	}

	SplineComponent = GetOwner()->FindComponentByClass<USplineComponent>();
	if (!IsValid(SplineComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("AutoRunComponent: No SplineComponent found on %s"), *GetOwner()->GetName());
	}
}


void UAutoRunComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentPawn) || !IsValid(SplineComponent) || !bIsAutoRunning)
	{
		return;
	}

	const FVector Location = SplineComponent->FindLocationClosestToWorldLocation(
		CurrentPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(
		Location, ESplineCoordinateSpace::World);
	
	CurrentPawn->AddMovementInput(Direction);
	if ((CachedDestination - Location).Length() <= AcceptanceRadius)
	{
		StopAutoRun();
	}
}
