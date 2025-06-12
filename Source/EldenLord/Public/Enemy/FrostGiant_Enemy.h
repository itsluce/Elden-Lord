// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "FrostGiant_Enemy.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()

class ELDENLORD_API AFrostGiant_Enemy : public AEnemy
{
	GENERATED_BODY()
public:
	AFrostGiant_Enemy();
	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
protected:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Weapon")
	UBoxComponent* WeaponBoxL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName WeaponBoxLAttachBoneName;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Weapon")
	UBoxComponent* WeaponBoxR;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName WeaponBoxRAttachBoneName;
public:
	FORCEINLINE UBoxComponent* GetWeaponBoxL() const {return WeaponBoxL;}
	FORCEINLINE UBoxComponent* GetWeaponBoxR() const {return WeaponBoxR;}
};
