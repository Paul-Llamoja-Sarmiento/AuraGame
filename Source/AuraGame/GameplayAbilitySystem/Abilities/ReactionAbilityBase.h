
#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "ReactionAbilityBase.generated.h"

/**
 * @brief Ability that reacts to external gameplay events.
 *
 * Abstract base for abilities not initiated directly by player input, but triggered by in-game events:
 // * - Listens for Gameplay Events or Tags to activate.
 // * - Can temporarily modify movement, input, or state.
 *
 * Examples of subclasses:
 * - UHitReactionAbilityBase: plays a hit-react animation when receiving damage.
 * - UStunReactionAbilityBase: responds when a stun effect is applied.
 */
UCLASS(Abstract)
class AURAGAME_API UReactionAbilityBase : public UGameplayAbilityBase
{
	GENERATED_BODY()


};
