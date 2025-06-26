
#include "CollectableHealthPotion.h"

#include "AbilitySystemInterface.h"
#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"


ACollectableHealthPotion::ACollectableHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACollectableHealthPotion::OnOverlapBegun(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change this to apply a gameplay effect. For now, using const_cast as a hack
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(
			ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(MutableAuraAttributeSet->GetHealth() + 25.f);
	}
	
	Super::OnOverlapBegun(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}


