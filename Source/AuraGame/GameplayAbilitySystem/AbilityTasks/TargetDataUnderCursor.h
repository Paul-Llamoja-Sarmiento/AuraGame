
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCursorTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);


/**
 * Ability Task that gathers a hit result under the mouse cursor
 * and sends it to the server as GameplayAbilityTargetData.
 * Works in both local prediction and server-authoritative scenarios.
 */
UCLASS()
class AURAGAME_API UTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCursorTargetDataDelegate OnTargetDataReady;

	/**
	 * Creates and starts the task to get a hit result under the mouse cursor and send it as target data.
	 * 
	 * @param OwningAbility The ability that owns this task.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "Target Data Under Cursor", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderCursor* CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;

private:
	void SendMouseCursorData() const;

	void ReceiveMouseCursorData();

	void OnTargetDataReplicatedHandle(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
	                                  FGameplayTag ActivationTag);
};
