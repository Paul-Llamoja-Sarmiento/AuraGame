
#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AuraGame/AuraGame.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CreateSphereComponent();	
	CreateProjectileMovementComponent();
	
	ProjectileAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ProjectileAudioComponent"));
}

void AProjectileBase::Destroyed()
{
	// Ensure impact effects are played on clients if the projectile was destroyed without registering a hit.
	// This handles cases where the projectile expires or is destroyed by other means,
	// and prevents missing visuals/audio in non-authoritative instances (clients).
	if (!bHasHitTarget && !HasAuthority())
	{
		ApplyImpactEffects();
	}
	
	Super::Destroyed();
}


void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(ProjectileLifeSpan);
	
	ProjectileAudioComponent->SetSound(LoopingSoundEffect);
	ProjectileAudioComponent->Play();

	SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnSphereOverlapped);
}


void AProjectileBase::OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (bHasHitTarget)
	{
		return;
	}
	
	if (OtherActor == GetOwner())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		                                 TEXT("Projectile hit owner, this shouldn't be happening! Ignoring."));
		return;
	}

	bHasHitTarget = true;
	ApplyImpactEffects();

	if (HasAuthority())
	{
		Destroy();
	}
}


void AProjectileBase::CreateSphereComponent()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void AProjectileBase::CreateProjectileMovementComponent()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}


void AProjectileBase::ApplyImpactEffects() const
{
	if (IsValid(ProjectileAudioComponent) && ProjectileAudioComponent->IsPlaying())
	{
		ProjectileAudioComponent->Stop();
	}
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSoundEffect, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVisualEffect, GetActorLocation());
}

