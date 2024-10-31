// luce copyright


#include "Actor/EldenEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/EldenAttributeSet.h"
#include "Components/SphereComponent.h"

AEldenEffectActor::AEldenEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AEldenEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEldenEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEldenEffectActor::EndOverlap);
}

void AEldenEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEldenEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UEldenAttributeSet* EldenAttributeSet = Cast<UEldenAttributeSet>(
			AscInterface->GetAbilitySystemComponent()->GetAttributeSet(UEldenAttributeSet::StaticClass()));

		UEldenAttributeSet* MutableEldenAttributeSet = const_cast<UEldenAttributeSet*>(EldenAttributeSet);
		MutableEldenAttributeSet->SetHealth(EldenAttributeSet->GetHealth() + 25.f);
		MutableEldenAttributeSet->SetMana(EldenAttributeSet->GetMana() - 25.f);
		MutableEldenAttributeSet->SetStamina(EldenAttributeSet->GetStamina() - 50.f);
		Destroy();
	}
}

void AEldenEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
