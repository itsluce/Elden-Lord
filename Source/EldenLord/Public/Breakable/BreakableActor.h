// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/CombatInterface.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"
UCLASS()
class ELDENLORD_API ABreakableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	// virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor*Hitter) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent*GeometryCollection;

};
