
#include "PlayerControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AuraGame/AuraGame.h"
#include "AuraGame/AuraGameplayTags.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGame/Input/AuraInputComponent.h"
#include "AuraGame/Interaction/HighlightableActor.h"
#include "Components/SplineComponent.h"


APlayerControllerBase::APlayerControllerBase()
{
	bReplicates = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

void APlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	check(MappingContext);

	auto MappingSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(MappingSubsystem))
	{
		MappingSubsystem->AddMappingContext(MappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::Move);
	AuraInputComponent->BindAbilityActions(InputConfiguration, this,
	                                       &ThisClass::AbilityInputPressed,
	                                       &ThisClass::AbilityInputReleased,
	                                       &ThisClass::AbilityInputHeld);
}


void APlayerControllerBase::AutoRun()
{
	APawn* ControlledPawn = GetPawn();
	if (!bAutoRunning || !IsValid(ControlledPawn))
	{
		return;
	}
	
	const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
	ControlledPawn->AddMovementInput(Direction);

	const float DistanceToDestination = (CachedDestination - LocationOnSpline).Length();
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
	}
}


void APlayerControllerBase::CursorTrace()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	if (!CursorHitResult.IsValidBlockingHit())
	{
		return;
	}

	LastHighlightedActor = CurrentHighlightedActor;
	CurrentHighlightedActor = CursorHitResult.GetActor();

	/*
	 * Line trace from the cursor position to find the actor under the cursor.
	 *
	 * Case A: LastHighlightedActor is nullptr && CurrentHighlightedActor is nullptr
	 *			- Do nothing.
	 * Case B: LastHighlightedActor is nullptr && CurrentHighlightedActor is not nullptr
	 *			- Highlight CurrentHighlightedActor.
	 * Case C: LastHighlightedActor is not nullptr && CurrentHighlightedActor is nullptr
	 * 			- UnHighlight LastHighlightedActor.
	 * Case D: Both actors are valid, but LastHighlightedActor != CurrentHighlightedActor
	 * 			- UnHighlight LastHighlightedActor.
	 * 			- Highlight CurrentHighlightedActor.
	 * Case E: Both actors are valid, but LastHighlightedActor == CurrentHighlightedActor
	 *			- Do nothing.
	 */

	if (LastHighlightedActor == nullptr)
	{
		if (CurrentHighlightedActor != nullptr)
		{
			IHighlightableActor::Execute_IHighlight(CurrentHighlightedActor.GetObject()); // Case B
		}
		else
		{
			// Case A
		}
	}
	else
	{
		if (CurrentHighlightedActor == nullptr)
		{
			IHighlightableActor::Execute_IUnHighlight(LastHighlightedActor.GetObject()); // Case C
		}
		else
		{
			if (LastHighlightedActor != CurrentHighlightedActor)
			{
				IHighlightableActor::Execute_IUnHighlight(LastHighlightedActor.GetObject()); // Case D
				IHighlightableActor::Execute_IHighlight(CurrentHighlightedActor.GetObject());
			}
			else
			{
				// Case E
			}
		}
	}
}

UAuraAbilitySystemComponent* APlayerControllerBase::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}

	return AuraAbilitySystemComponent;
}


void APlayerControllerBase::Move(const FInputActionValue& InputActionValue)
{
	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn))
	{
		return;
	}

	const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	ControlledPawn->AddMovementInput(ForwardDirection, MoveValue.Y);
	ControlledPawn->AddMovementInput(RightDirection, MoveValue.X);
}


void APlayerControllerBase::AbilityInputPressed(const FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(Input_RMB))
	{
		bIsTargeting = CurrentHighlightedActor ? true : false;
		bAutoRunning = false;
	}
}

void APlayerControllerBase::AbilityInputHeld(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(Input_RMB) || bIsTargeting)
	{
		if (GetAuraASC() != nullptr)
		{
			AuraAbilitySystemComponent->AbilityInputHeldHandle(InputTag);;
		}
		
		return;
	}

	FollowTime += GetWorld()->GetDeltaSeconds();
	FHitResult CursorHitResult;
	if (GetHitResultUnderCursor(ECC_Navigation, false, CursorHitResult))
	{
		CachedDestination = CursorHitResult.ImpactPoint;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
	}
	
}

void APlayerControllerBase::AbilityInputReleased(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(Input_RMB) || bIsTargeting)
	{
		if (GetAuraASC() != nullptr)
		{
			AuraAbilitySystemComponent->AbilityInputReleasedHandle(InputTag);;
		}
		
		return;
	}

	// If the player has held the left mouse button for a short time, we will start auto-running.
	const APawn* ControlledPawn = GetPawn();
	if (FollowTime <= ShortPressThresholdInSeconds && IsValid(ControlledPawn) && IsValid(NavSystem))
	{
		FHitResult NavChannelCursorHitResult;
		GetHitResultUnderCursor(ECC_Navigation, false, NavChannelCursorHitResult);
		if (NavChannelCursorHitResult.bBlockingHit)
		{
			// Projecting a point from the cursor impact point to the NavMesh with a larger-than-default Query Extent,
			// so there are better chances to reach for the NavMesh and return a point.
			// Then, generate a path from the pawn location to this point (only if found).
			// NOTE: Default QueryExtend = FVector(50.0f, 50.0f, 250.0f), but we will be using a larger one to ensure
			// that we can find a point on the NavMesh even if the cursor is far away from it. This is suitable for
			// our level design, but it may be adjusted different if needed
			FNavLocation ImpactPointNavLocation;
			const FVector QueryingExtent = FVector(400.0f, 400.0f, 250.0f);
			const FNavAgentProperties& NavAgentProps = GetNavAgentPropertiesRef();
			const bool bNavLocationFound = NavSystem->ProjectPointToNavigation(NavChannelCursorHitResult.ImpactPoint,
			                                                                   ImpactPointNavLocation, QueryingExtent,
			                                                                   &NavAgentProps);
			if (bNavLocationFound)
			{
				UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
					GetWorld(), ControlledPawn->GetActorLocation(), ImpactPointNavLocation.Location);
				if (NavPath && NavPath->PathPoints.Num() > 1 && IsValid(SplineComponent))
				{
					SplineComponent->ClearSplinePoints();
					for (const FVector& PointLocation : NavPath->PathPoints)
					{
						SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
					}

					CachedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}			
		}		
	}

	bIsTargeting = false;
	FollowTime = 0.f;
}
