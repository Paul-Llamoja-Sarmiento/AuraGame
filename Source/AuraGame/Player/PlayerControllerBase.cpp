
#include "PlayerControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AuraGame/AuraGame.h"
#include "AuraGame/AuraGameplayTags.h"
#include "AuraGame/Components/AutoRunComponent.h"
#include "AuraGame/Components/HighlightingComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGame/Input/AuraInputComponent.h"


APlayerControllerBase::APlayerControllerBase()
{
	bReplicates = true;
	
	AutoRunComponent = CreateDefaultSubobject<UAutoRunComponent>(TEXT("AutoRunComponent"));
	HighlightingComponent = CreateDefaultSubobject<UHighlightingComponent>(TEXT("HighlightingComponent"));
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

	ConfigureInputMode();
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


UAuraAbilitySystemComponent* APlayerControllerBase::GetAuraASC()
{
	// TODO: AuraAbilitySystemComponent may be null or change if the controlled pawn changes.
	// Consider caching it or updating it when the pawn changes. OnPossess and OnUnPossess are good candidates for this,
	// but they are called on the server, so we need to ensure the client has the correct reference.
	if (!IsValid(AuraAbilitySystemComponent))
	{
		APawn* ControlledPawn = GetPawn();
		if (IsValid(ControlledPawn))
		{
			if (auto* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn))
			{
				AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(ASC);
				if (!IsValid(AuraAbilitySystemComponent))
				{
					UE_LOG(LogTemp, Warning, TEXT("Pawn %s does not have UAuraAbilitySystemComponent"), 
						  *ControlledPawn->GetName());
				}
			}
		}
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

	if (AutoRunComponent->IsAutoRunning())
	{
		AutoRunComponent->StopAutoRun();
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
		bIsTargeting = HighlightingComponent->HasCurrentHighlightedActor();
		if (AutoRunComponent->IsAutoRunning())
		{
			AutoRunComponent->StopAutoRun();
		}
	}
}

void APlayerControllerBase::AbilityInputHeld(const FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(Input_RMB) || bIsTargeting)
	{
		if (GetAuraASC() != nullptr)
		{
			AuraAbilitySystemComponent->AbilityInputHeldHandle(InputTag);
		}
		
		return;
	}

	HandleDirectMovementInput();
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
	APawn* ControlledPawn = GetPawn();
	if (FollowTime <= ShortPressThresholdInSeconds && IsValid(ControlledPawn))
	{
		AutoRunComponent->TryAutoRunToCursorLocation(this);
	}

	bIsTargeting = false;
	FollowTime = 0.f;
}

void APlayerControllerBase::HandleDirectMovementInput()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	FHitResult CursorHitResult;
	if (GetHitResultUnderCursor(ECC_Navigation, false, CursorHitResult))
	{
		APawn* ControlledPawn = GetPawn();
		if (IsValid(ControlledPawn))
		{
			const FVector WorldDirection = (CursorHitResult.ImpactPoint - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}	
}

void APlayerControllerBase::ConfigureInputMode()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}
