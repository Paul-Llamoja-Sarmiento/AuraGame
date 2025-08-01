
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "HitReactionHandlerComponent.generated.h"


class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UHitReactionHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitReactionHandlerComponent();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure)
	bool IsHitReactionActive() const { return bIsHitReacting; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC;

	FDelegateHandle HitReactionTagHandle;

	bool bIsHitReacting = false;

	void BindToHitReactionTagEvent();

	void UnbindFromHitReactionTagEvent();

	void OnHitReactionTagChangedHandle(const FGameplayTag CallbackTag, int32 NewCount);
};
