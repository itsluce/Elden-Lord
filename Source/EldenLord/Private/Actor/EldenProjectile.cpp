// luce copyright


#include "Actor/EldenProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "EldenLord/EldenLord.h"
#include "Enemy/Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

AEldenProjectile::AEldenProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->InitialSpeed = 550.f;
	Projectile->MaxSpeed = 550.f;
	Projectile->ProjectileGravityScale = 0.f;
}

void AEldenProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator);
		LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

void AEldenProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEldenProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopSound, GetRootComponent());
}

void AEldenProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	// Prevent enemy-to-enemy damage
	APawn* ProjectileOwner = GetInstigator<APawn>();
	if (ProjectileOwner && OtherActor)
	{
		AEnemy* OwnerEnemy = Cast<AEnemy>(ProjectileOwner);
		AEnemy* HitEnemy = Cast<AEnemy>(OtherActor);
		
		// If both are enemies, don't allow damage
		if (OwnerEnemy && HitEnemy)
		{
			return;
		}
		
		// Don't hit self
		if (ProjectileOwner == OtherActor)
		{
			return;
		}
	}

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor);
	CombatInterface->Execute_GetImpactAngle(OtherActor, SweepResult.ImpactPoint, OtherActor);

	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator);
	LoopingSoundComponent->Stop();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

float AEldenProjectile::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);
	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	return Theta;
	//
	// FName Section("FromBack");
	//
	// if (Theta >= -45.f && Theta < 45.f)
	// {
	// 	Section = FName("FromFront");
	// }
	// else if (Theta >= -135.f && Theta < -45.f)
	// {
	// 	Section = FName("FromLeft");
	// }
	// else if (Theta >= 45.f && Theta < 135.f)
	// {
	// 	Section = FName("FromRight");
	// }
	//
	// PlayHitReactMontage(Section);
}
