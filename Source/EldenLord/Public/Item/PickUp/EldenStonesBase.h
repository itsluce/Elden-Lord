// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Item/PickUp/EldenPickUpBase.h"
#include "EldenStonesBase.generated.h"

class UGameplayEffect;
class UEldenAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenStonesBase : public AEldenPickUpBase
{
	GENERATED_BODY()
	
public:
	void Consume(UEldenAbilitySystemComponent* AbilitySystemComponent,int32 ApplyLevel);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;
};
