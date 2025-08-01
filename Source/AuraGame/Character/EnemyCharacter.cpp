
#include "EnemyCharacter.h"

#include "AuraGame/AuraGame.h"
#include "AuraGame/Components/CharacterAssetComponent.h"
#include "AuraGame/GameplayAbilitySystem/AbilityBlueprintFunctionLibrary.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"
#include "AuraGame/UI/WidgetController/EnemyWidgetController.h"
#include "AuraGame/UI/WidgetController/Base/WidgetControllerBase.h"
#include "Components/WidgetComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	// Mesh must block Visibility to be hit by cursor traces (e.g., mouse hover).
	// Consider creating a custom collision preset if more changes are needed.
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	CreateGameplayAbilitySystemObjects();

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	CharacterAssetsComponent = CreateDefaultSubobject<UCharacterAssetComponent>(TEXT("CharacterAssetsComponent"));
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(EnemyWidgetController))
	{
		EnemyWidgetController->CleanupController();
		EnemyWidgetController = nullptr;
	}
	
	Super::EndPlay(EndPlayReason);
}

void AEnemyCharacter::IHighlight_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	WeaponMesh->SetRenderCustomDepth(true);
}

void AEnemyCharacter::IUnHighlight_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
}

UCharacterAssetsInfo* AEnemyCharacter::IGetCharacterAssetsInfo_Implementation() const
{
	return CharacterAssetsComponent->GetCharacterAssetsInfo();
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	InitializeAbilityActorInfo();
	UAbilityBlueprintFunctionLibrary::InitializeStartupAbilities(GetWorld(), AbilitySystemComponent);
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UAbilityBlueprintFunctionLibrary::InitializeDefaultAttributes(GetWorld(), CharacterClass, Level, AbilitySystemComponent);
}

void AEnemyCharacter::InitializeAbilityActorInfo()
{
	// Initialize ASC with this actor as both Owner and Avatar.
	// Done in BeginPlay because AI characters don’t have full context during construction.
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();

	// Initialize health bar widget after ASC is set up.
	InitializeHealthWidgetBar();
	
	InitializeDefaultAttributes();
}

void AEnemyCharacter::InitializeHealthWidgetBar()
{
	checkf(EnemyWidgetControllerClass, TEXT("EnemyWidgetControllerClass must be set in EnemyCharacter!"));

	UUserWidget* HealthBarUserWidget = HealthBarWidget->GetUserWidgetObject();
	if (!IsValid(HealthBarUserWidget))
	{
		return;
	}
	
	const FWidgetControllerParams Params(HealthBarUserWidget, nullptr, nullptr, AbilitySystemComponent, AttributeSet);
	EnemyWidgetController = NewObject<UEnemyWidgetController>(this, EnemyWidgetControllerClass);
	EnemyWidgetController->InitializeAuraWidgetController(Params);
}

void AEnemyCharacter::CreateGameplayAbilitySystemObjects()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// "Minimal" is enough for AI: only replicates essential effects, reducing network cost.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}
