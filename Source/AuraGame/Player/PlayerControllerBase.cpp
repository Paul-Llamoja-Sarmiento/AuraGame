
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


void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AutoRunComponent = InPawn ? InPawn->FindComponentByClass<UAutoRunComponent>() : nullptr;
	AuraAbilitySystemComponent = GetAuraASC();
}


void APlayerControllerBase::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	AutoRunComponent = GetPawn() ? GetPawn()->FindComponentByClass<UAutoRunComponent>() : nullptr;
	AuraAbilitySystemComponent = GetAuraASC();
}


UAuraAbilitySystemComponent* APlayerControllerBase::GetAuraASC()
{
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

	StopAutoRunMovement();
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
		StopAutoRunMovement();
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
	if (FollowTime <= ShortPressThresholdInSeconds && IsValid(ControlledPawn) && IsValid(AutoRunComponent))
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


void APlayerControllerBase::StopAutoRunMovement() const
{
	if (IsValid(AutoRunComponent) && AutoRunComponent->IsAutoRunning())
	{
		AutoRunComponent->StopAutoRun();
	}
}
