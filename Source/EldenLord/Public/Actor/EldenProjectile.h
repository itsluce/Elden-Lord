// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EldenProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ELDENLORD_API AEldenProjectile : public AActor
{
	GENERATED_BODY()

public:
	AEldenProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> Projectile;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
