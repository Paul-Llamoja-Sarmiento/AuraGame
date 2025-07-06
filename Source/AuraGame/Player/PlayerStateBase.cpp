
#include "PlayerStateBase.h"

#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"

APlayerStateBase::APlayerStateBase()
{
	// GAS requires frequent net updates for abilities and effects to sync properly.
	SetNetUpdateFrequency(100.0f);
	CreateGameplayAbilitySystemObjects();
}

void APlayerStateBase::CreateGameplayAbilitySystemObjects()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// "Mixed" replication balances performance and accuracy for player-controlled characters.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}
