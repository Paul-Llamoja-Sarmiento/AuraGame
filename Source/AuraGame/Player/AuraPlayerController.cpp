
#include "AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AuraGame/Interaction/HighlightableActor.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraMappingContext);

	auto MappingSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(MappingSubsystem))
	{
		MappingSubsystem->AddMappingContext(AuraMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(AuraMoveInputAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
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

void AAuraPlayerController::CursorTrace()
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
