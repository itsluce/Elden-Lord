// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate,AActor*)
UCLASS()
class ELDENLORD_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	FOnTargetInteractedDelegate OnWeaponHitTarget;
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
	
protected:
	UFUNCTION()
	void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Weapon")
	UBoxComponent* WeaponBox;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
