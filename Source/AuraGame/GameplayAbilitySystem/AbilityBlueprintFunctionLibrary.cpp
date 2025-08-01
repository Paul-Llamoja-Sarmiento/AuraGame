
#include "AbilityBlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraGame/Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UAbilityBlueprintFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                          ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	const UCharacterClassInfo* CharactersClassInfo = GetCharacterClassInfoFromGameMode(WorldContextObject);
	if (!IsValid(CharactersClassInfo) || !IsValid(ASC))
	{
		return; 
	}
	
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharactersClassInfo->GetClassDefaultsInfo(CharacterClass);

	ApplyEffectToASC(ASC, CharacterClassDefaultInfo.DefaultPrimaryAttributes, Level);
	ApplyEffectToASC(ASC, CharactersClassInfo->DefaultSecondaryAttributes, Level);
	ApplyEffectToASC(ASC, CharactersClassInfo->DefaultVitalAttributes, Level);
}

void UAbilityBlueprintFunctionLibrary::InitializeStartupAbilities(const UObject* WorldContextObject,
                                                         UAbilitySystemComponent* ASC)
{
	const UCharacterClassInfo* CharactersClassInfo = GetCharacterClassInfoFromGameMode(WorldContextObject);
	if (!IsValid(CharactersClassInfo) || !IsValid(ASC))
	{
		return;
	}

	for (const auto& AbilityClass : CharactersClassInfo->DefaultCommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

void UAbilityBlueprintFunctionLibrary::ApplyEffectToASC(UAbilitySystemComponent* ASC,
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

UCharacterClassInfo* UAbilityBlueprintFunctionLibrary::GetCharacterClassInfoFromGameMode(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(AuraGameMode))
	{
		return nullptr;
	}

	return AuraGameMode->GetCharacterClassInfo();
}
