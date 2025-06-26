
#include "EnemyCharacter.h"

#include "AuraGame/AuraGame.h"
#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"


AEnemyCharacter::AEnemyCharacter()
{
	// Mesh must block Visibility to be hit by cursor traces (e.g., mouse hover).
	// Consider creating a custom collision preset if more changes are needed.
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CreateGameplayAbilitySystemObjects();
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

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// Initialize ASC with this actor as both Owner and Avatar.
	// Done in BeginPlay because AI characters don’t have full context during construction.
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AEnemyCharacter::CreateGameplayAbilitySystemObjects()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// "Minimal" is enough for AI: only replicates essential effects, reducing network cost.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}
