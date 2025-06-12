// luce copyright


#include "Enemy/FrostGiant_Enemy.h"

#include "Components/BoxComponent.h"
#include "Components/EnemyCombatComponent.h"

AFrostGiant_Enemy::AFrostGiant_Enemy()
{
	WeaponBoxL = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	WeaponBoxL->SetupAttachment(GetMesh());
	WeaponBoxL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxL->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);

	WeaponBoxR = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	WeaponBoxR->SetupAttachment(GetMesh());
	WeaponBoxR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxR->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnBodyCollisionBoxBeginOverlap);
}

void AFrostGiant_Enemy::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (this != HitPawn)
		{
			EnemyCombatComponent->OnHitTargetActor(HitPawn);
		}
	}
}

void AFrostGiant_Enemy::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,WeaponBoxLAttachBoneName))
	{
		WeaponBoxL->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,WeaponBoxLAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass,WeaponBoxRAttachBoneName))
	{
		WeaponBoxR->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,WeaponBoxRAttachBoneName);
	}
}
