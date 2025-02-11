// luce copyright


#include "Item/Weapon/Weapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenGameplayTags.h"
#include "NiagaraComponent.h"
#include "Character/EldenLordCharacter.h"
#include "Components/BoxComponent.h"
#include "EldenDbug.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionResponseToChannels(ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;

	// Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	// Sphere->SetupAttachment(GetRootComponent());

	WeaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	WeaponEffect->SetupAttachment(GetRootComponent());
	
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	BoxTracStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trac Start"));
	BoxTracStart->SetupAttachment(GetRootComponent());
	BoxTracEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trac End"));
	BoxTracEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnBoxEndOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
}

void AWeapon::DisableSphereCollision()
{
	// if (Sphere)
	// {
	// 	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// }
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor)) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		if (ActorIsSameType(BoxHit.GetActor())) return;

		UE_LOG(LogTemp, Log, TEXT("BoxHit.ImpactPoint: %s"), *BoxHit.ImpactPoint.ToString());

		FGameplayEventData EventData;
		EventData.Instigator = GetInstigator();
		EventData.Target = BoxHit.GetActor();
		EventData.ContextHandle = FGameplayEffectContextHandle();
		EventData.EventMagnitude = Damage;

		FGameplayAbilityTargetData_LocationInfo* LocationData = new FGameplayAbilityTargetData_LocationInfo();
		LocationData->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
		LocationData->TargetLocation.LiteralTransform = FTransform(BoxHit.ImpactPoint);
		EventData.TargetData.Add(LocationData);

		UE_LOG(LogTemp, Log, TEXT("EventData.TargetData.Num(): %d"), EventData.TargetData.Num());
		for (const TSharedPtr<FGameplayAbilityTargetData>& TargetData : EventData.TargetData.Data)
		{
			if (TargetData.IsValid())
			{
				UE_LOG(LogTemp, Log, TEXT("TargetData Type: %s"), *TargetData->GetScriptStruct()->GetName());
			}
		}

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), FGameplayTag::RequestGameplayTag("Event.Weapon.Impact"), EventData);

		CreateField(BoxHit.ImpactPoint);
	}
}
void AWeapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

	IgnoreActors.Remove(OtherActor);
}
bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult BoxHit)
{
	ICombatInterface* HitInterface = Cast<ICombatInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint,GetOwner());
	}
}


void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTracStart->GetComponentLocation();
	const FVector End = BoxTracEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.AddUnique(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtend,
		BoxTracStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowDebugBox ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
