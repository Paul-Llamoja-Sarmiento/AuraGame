
#include "AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/Player/AuraPlayerStateBase.h"
#include "AuraGame/UI/HUD/AuraHUDInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetSystemLibrary.h"


AAuraCharacter::AAuraCharacter()
{
	// Basic setup for top-down character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Called only on the server when this character is possessed.
	// Initializes ASC with proper owner/avatar info for server-side logic.
	InitializeAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Called on the owning client when PlayerState is replicated.
	// Initializes ASC with correct info for client-side prediction and UI.
	InitializeAbilityActorInfo();
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->RotationRate = FRotator(0.0f, BaseTurnRate, 0.0f);
}

void AAuraCharacter::InitializeAbilityActorInfo()
{
	AAuraPlayerStateBase* AuraPlayerState = GetPlayerState<AAuraPlayerStateBase>();
	check(IsValid(AuraPlayerState));
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// Initialize the HUD overlay now that ASC, AttributeSet, and PlayerState are fully initialized.
	// We do this here to avoid re-fetching these references elsewhere.
	InitializeHUD();
}

void AAuraCharacter::InitializeHUD() const
{
	APlayerController* PC = GetController<APlayerController>();
	if (!IsValid(PC))
	{
		return;
	}

	AHUD* HUD = PC->GetHUD();
	if (!IsValid(HUD) || !UKismetSystemLibrary::DoesImplementInterface(HUD, UAuraHUDInterface::StaticClass()))
	{
		return;
	}

	IAuraHUDInterface::Execute_IInitOverlay(HUD, PC, GetPlayerState(), AbilitySystemComponent, AttributeSet);
}
