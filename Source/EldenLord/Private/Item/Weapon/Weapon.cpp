// luce copyright


#include "Item/Weapon/Weapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenGameplayTags.h"
#include "NiagaraComponent.h"
#include "Character/EldenLordCharacter.h"
#include "Components/BoxComponent.h"
#include "EldenDbug.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetBoxExtent(FVector(20.f));
	WeaponBox->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBoxEndOverlap);
}

void AWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn,TEXT("Forgot to assign an instiagtor as the owning pawn for the weapon: %s"),*GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UEldenAbilitySystemLibrary::IsTargetPawnHostile(WeaponOwningPawn,HitPawn))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void AWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn,TEXT("Forgot to assign an instiagtor as the owning pawn for the weapon: %s"),*GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UEldenAbilitySystemLibrary::IsTargetPawnHostile(WeaponOwningPawn,HitPawn))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}