
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURAGAME_API AEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AEffectActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass) const;
};
