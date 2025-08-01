
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"


class UGameplayAbility;
class UGameplayEffect;


/**
 * @enum ECharacterClass
 * @brief Enumerates the available character archetypes for enemy initialization.
 *
 * Used by UCharacterClassInfo to map class-specific default gameplay effects.
 */
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	// Melee-oriented warrior with high durability.
	Warrior = 0,

	//Caster-focused mage with spell-based attacks.
	Mage,

	// Ranged combatant relying on high damage.
	Ranger
};


/**
 * @struct FCharacterClassDefaultInfo
 * @brief Holds primary attribute defaults for a specific character class.
 *
 * Associates a GameplayEffect subclass to initialize primary attributes when
 * spawning or resetting an enemy of this class.
 */
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	// GameplayEffect to apply primary attributes specific to this class.
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
};


/**
 * @class UCharacterClassInfo
 * @brief Central repository for default gameplay effects per character class.
 *
 * Represents a single source of truth for class-specific default attributes
 * during character initialization.
 */
UCLASS()
class AURAGAME_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// Class-specific defaults: maps each archetype enum to its primary attributes effect.
	UPROPERTY(EditDefaultsOnly, Category = "Exclusive Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassDefaultsInfo;

	// Default GameplayEffect for secondary attributes (e.g., armor, block chance).
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// Default GameplayEffect for vital attributes (e.g., health, mana).
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// Array of default common abilities that all characters should have (e.g., hit reaction).
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> DefaultCommonAbilities;

	
	/**
	  * Retrieves the FCharacterClassDefaultInfo for a given class.
	  *
	  * @param CharacterClass The archetype enum value to lookup.
	  * @return The struct containing the primary attribute effect.
	  */
	FCharacterClassDefaultInfo GetClassDefaultsInfo(ECharacterClass CharacterClass) const;
};
