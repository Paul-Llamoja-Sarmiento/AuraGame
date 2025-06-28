
#include "AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/Player/AuraPlayerStateBase.h"
#include "GameFramework/CharacterMovementComponent.h"


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
}
