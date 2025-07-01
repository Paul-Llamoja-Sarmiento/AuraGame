
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectApplierComponent.generated.h"


class UAbilitySystemComponent;
class UGameplayEffect;
struct FActiveGameplayEffectHandle;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	DoNotApply     UMETA(DisplayName = "Don't Apply"),
	ApplyOnOverlap UMETA(DisplayName = "Apply on Begin Overlap"),
	ApplyOnEndOverlap UMETA(DisplayName = "Apply on End Overlap")
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	DoNotRemove      UMETA(DisplayName = "Don't Remove"),
	RemoveOnEndOverlap UMETA(DisplayName = "Remove on End Overlap")
};

USTRUCT(BlueprintType)
struct FEffectApplicationConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::ApplyOnOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS",
		meta = (EditCondition = "ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap", EditConditionHides,
			ToolTip = "Only relevant if the effect has an infinite duration. Ignored otherwise."))
	EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURAGAME_API UGameplayEffectApplierComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameplayEffectApplierComponent();

protected:
	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	TArray<FEffectApplicationConfig> EffectsToApply;

	// TODO: Consider using a weak pointer if a stale pointer arises
	TMap<UAbilitySystemComponent*, TArray<FActiveGameplayEffectHandle>> ActiveEffectsMap;

	UPROPERTY(EditAnywhere, Category = "Applied Effects",
		meta = (ToolTip =
			"Must tag the collision volume component of the owner actor with this name. It won't work otherwise."))
	FName CollisionVolumeTag = "effect-applier-collision-volume";

	UPROPERTY(EditAnywhere, Category = "Applied Effects")
	bool bShouldDestroyAfterApplyEffects = true;

	UFUNCTION()
	void OnBeginOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

	void ApplyEffectToTarget(UAbilitySystemComponent* TargetASC, const FEffectApplicationConfig& EffectConfig);

	void RemoveEffectsFromTarget(UAbilitySystemComponent* TargetASC);
};
