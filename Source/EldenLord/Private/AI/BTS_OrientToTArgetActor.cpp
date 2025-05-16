// luce copyright


#include "AI/BTS_OrientToTArgetActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UBTS_OrientToTArgetActor::UBTS_OrientToTArgetActor()
{
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationInterpSpeed = 5.f;
	Interval = 0.f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UBTS_OrientToTArgetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTS_OrientToTArgetActor::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTS_OrientToTArgetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	const bool bStun = UEldenAbilitySystemLibrary::NativeDoesActorHaveTag(
		OwningPawn, FEldenGameplayTags::Get().Enemy_Status_KnockedDown);

	if (OwningPawn && TargetActor && !bStun)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds,
		                                            RotationInterpSpeed);

		OwningPawn->SetActorRotation(TargetRot);
	}
}
