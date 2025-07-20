
#include "PlayerControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
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
	if (InputTag.MatchesTagExact(Input_LMB))
	{
		bIsTargeting = CurrentHighlightedActor ? true : false;
		bAutoRunning = false;
	}
}

void APlayerControllerBase::AbilityInputHeld(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(Input_LMB) || bIsTargeting)
	{
		if (GetAuraASC() != nullptr)
		{
			AuraAbilitySystemComponent->AbilityInputHeldHandle(InputTag);;
		}
		
		return;
	}

	FollowTime += GetWorld()->GetDeltaSeconds();
	FHitResult CursorHitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult))
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
	if (!InputTag.MatchesTagExact(Input_LMB) || bIsTargeting)
	{
		if (GetAuraASC() != nullptr)
		{
			AuraAbilitySystemComponent->AbilityInputReleasedHandle(InputTag);;
		}
		
		return;
	}

	FollowTime = 0.f;
	bIsTargeting = false;
	// If the player has held the left mouse button for a short time, we will start auto-running.
	const APawn* ControlledPawn = GetPawn();
	if (FollowTime > ShortPressThresholdInSeconds || !IsValid(ControlledPawn))
	{
		return;
	}

	if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		this, ControlledPawn->GetActorLocation(), CachedDestination))
	{
		SplineComponent->ClearSplinePoints();
		if (NavPath->PathPoints.Num() == 0)
		{
			return;
		}
		
		bAutoRunning = true;
		for (const FVector& PointLocation : NavPath->PathPoints)
		{
			SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
			DrawDebugSphere(GetWorld(), PointLocation, 10.f, 12, FColor::Red, false, 5.f);
		}
	}
}
