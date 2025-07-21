
#include "HighlightingComponent.h"

#include "AuraGame/Interaction/HighlightableActor.h"


UHighlightingComponent::UHighlightingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHighlightingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CachedPlayerController.IsValid())
	{
		PerformCursorTrace();
	}
}

void UHighlightingComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayerController = Cast<APlayerController>(GetOwner());
	SetComponentTickEnabled(CachedPlayerController.IsValid());
}

void UHighlightingComponent::PerformCursorTrace()
{
	LastHighlightedActor = CurrentHighlightedActor;
	
	FHitResult CursorHitResult;
	CachedPlayerController->GetHitResultUnderCursor(TraceChannel, false, CursorHitResult);
	if (!CursorHitResult.IsValidBlockingHit())
	{
		CurrentHighlightedActor = nullptr;
		return;
	}
	
	CurrentHighlightedActor = CursorHitResult.GetActor();
	if (LastHighlightedActor != CurrentHighlightedActor)
	{
		if (LastHighlightedActor)
		{
			IHighlightableActor::Execute_IUnHighlight(LastHighlightedActor.GetObject());
		}

		if (CurrentHighlightedActor)
		{
			IHighlightableActor::Execute_IHighlight(CurrentHighlightedActor.GetObject());
		}
	}	
}
