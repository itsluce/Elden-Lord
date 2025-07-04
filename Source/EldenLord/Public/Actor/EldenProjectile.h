// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Interface/CombatInterface.h"
#include "EldenProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ELDENLORD_API AEldenProjectile : public AActor
{
	GENERATED_BODY()

public:
	AEldenProjectile();
	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> Projectile;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	void PlayHitReactMontage(FName Section );
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	float DirectionalHitReact(const FVector& ImpactPoint);
	
	bool bHit = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
};
