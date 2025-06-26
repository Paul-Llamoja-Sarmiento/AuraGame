
#include "AuraCollectableActorBase.h"

#include "Components/SphereComponent.h"


AAuraCollectableActorBase::AAuraCollectableActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(RootComponent);
}

void AAuraCollectableActorBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraCollectableActorBase::OnOverlapBegun);
}

void AAuraCollectableActorBase::OnOverlapBegun(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Destroy();
}

