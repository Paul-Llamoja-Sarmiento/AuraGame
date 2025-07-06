
#include "PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/Player/PlayerStateBase.h"
#include "AuraGame/UI/HUD/HUDInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetSystemLibrary.h"


APlayerCharacter::APlayerCharacter()
{
	// Basic setup for top-down character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Called only on the server when this character is possessed.
	// Initializes ASC with proper owner/avatar info for server-side logic.
	InitializeAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Called on the owning client when PlayerState is replicated.
	// Initializes ASC with correct info for client-side prediction and UI.
	InitializeAbilityActorInfo();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->RotationRate = FRotator(0.0f, BaseTurnRate, 0.0f);
}

void APlayerCharacter::InitializeAbilityActorInfo()
{
	APlayerStateBase* AuraPlayerState = GetPlayerState<APlayerStateBase>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// Initialize the HUD overlay now that ASC, AttributeSet, and PlayerState are fully initialized.
	// We do this here to avoid re-fetching these references elsewhere.
	InitializeHUD();
}

void APlayerCharacter::InitializeHUD() const
{
	APlayerController* PC = GetController<APlayerController>();
	if (!IsValid(PC))
	{
		return;
	}

	AHUD* HUD = PC->GetHUD();
	if (!IsValid(HUD) || !UKismetSystemLibrary::DoesImplementInterface(HUD, UHUDInterface::StaticClass()))
	{
		return;
	}

	IHUDInterface::Execute_IInitOverlay(HUD, PC, GetPlayerState(), AbilitySystemComponent, AttributeSet);
}
