// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DisableSphereCollision();
	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult BoxHit);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

private:
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USphereComponent* Sphere;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USceneComponent* BoxTracStart;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USceneComponent* BoxTracEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Weapon")
	UNiagaraComponent* WeaponEffect;
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	bool bShowDebugBox = false;

	UPROPERTY
	(EditAnywhere, Category="Weapon")
	FVector BoxTraceExtend = FVector(5.f);

	void BoxTrace(FHitResult& BoxHit);

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
