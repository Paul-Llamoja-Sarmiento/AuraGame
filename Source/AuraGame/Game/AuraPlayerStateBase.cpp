// 


#include "AuraPlayerStateBase.h"

#include "AuraGame/GameplayAbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"

AAuraPlayerStateBase::AAuraPlayerStateBase()
{
	// GAS requires frequent net updates for abilities and effects to sync properly.
	SetNetUpdateFrequency(100.0f);
	CreateGameplayAbilitySystemObjects();
}

void AAuraPlayerStateBase::CreateGameplayAbilitySystemObjects()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// "Mixed" replication balances performance and accuracy for player-controlled characters.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}
