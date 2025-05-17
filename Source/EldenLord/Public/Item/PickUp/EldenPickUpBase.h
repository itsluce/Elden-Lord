// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EldenPickUpBase.generated.h"

class USphereComponent;

UCLASS()
class ELDENLORD_API AEldenPickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEldenPickUpBase();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick UP Interaction")
	USphereComponent* PickUpCollisionSphere;

	UFUNCTION()
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
