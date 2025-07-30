
#include "AbilityFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UAbilityFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                          ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(AuraGameMode) || !IsValid(ASC))
	{
		return;
	}

	const UCharacterClassInfo* CharactersClassInfo = AuraGameMode->GetCharacterClassInfo();
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharactersClassInfo->GetClassDefaultsInfo(CharacterClass);

	ApplyEffectToASC(ASC, CharacterClassDefaultInfo.DefaultPrimaryAttributes, Level);
	ApplyEffectToASC(ASC, CharactersClassInfo->DefaultSecondaryAttributes, Level);
	ApplyEffectToASC(ASC, CharactersClassInfo->DefaultVitalAttributes, Level);
}

void UAbilityFunctionLibrary::ApplyEffectToASC(UAbilitySystemComponent* ASC,
                                               const TSubclassOf<UGameplayEffect>& GameplayEffectClass,
                                               const float Level)
{
	check(GameplayEffectClass);
	if (!IsValid(ASC))
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(
		GameplayEffectClass, Level, EffectContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}
