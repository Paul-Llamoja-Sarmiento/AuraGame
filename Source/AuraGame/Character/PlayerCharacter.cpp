
#include "PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/Components/AutoRunComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGame/Player/PlayerStateBase.h"
#include "AuraGame/UI/HUD/HUDInterface.h"
#include "Components/SplineComponent.h"
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

	AutoRunComponent = CreateDefaultSubobject<UAutoRunComponent>(TEXT("AutoRunComponent"));
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Called only on the server when this character is possessed.
	// Initializes ASC with proper owner/avatar info for server-side logic.
	InitializeAbilityActorInfo();
	AddCharacterAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Called on the owning client when PlayerState is replicated.
	// Initializes ASC with correct info for client-side prediction and UI.
	InitializeAbilityActorInfo();
}

int32 APlayerCharacter::IGetCharacterLevel_Implementation() const
{
	APlayerState* CurrentPlayerState = GetPlayerState();
	if (IsValid(CurrentPlayerState) && CurrentPlayerState->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
	{
		return ICombatInterface::Execute_IGetCharacterLevel(CurrentPlayerState);
	}

	return 1; // Default level if PlayerState is invalid or doesn't implement ICombatInterface
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->RotationRate = FRotator(0.0f, BaseTurnRate, 0.0f);
}

void APlayerCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesEffect);
	ApplyEffectToSelf(DefaultSecondaryAttributesEffect);
	ApplyEffectToSelf(DefaultVitalAttributesEffect);
}

void APlayerCharacter::InitializeAbilityActorInfo()
{
	APlayerStateBase* AuraPlayerState = GetPlayerState<APlayerStateBase>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AttributeSet = AuraPlayerState->GetAttributeSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (IsValid(AuraASC))
	{
		AuraASC->OnAbilityActorInfoSet();
	}
	
	// Initialize the HUD overlay now that ASC, AttributeSet, and PlayerState are fully initialized.
	// We do this here to avoid re-fetching these references elsewhere.
	InitializeHUD();

	InitializeDefaultAttributes();
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

void APlayerCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, float InLevel) const
{
	check(AbilitySystemComponent);

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		EffectClass, InLevel, EffectContextHandle);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}

void APlayerCharacter::AddCharacterAbilities() const
{
	if (!HasAuthority())
	{
		return;
	}

	const auto AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!IsValid(AuraAbilitySystemComponent))
	{
		return;
	}

	AuraAbilitySystemComponent->GrantCharacterAbilities(StartupAbilities);
}
