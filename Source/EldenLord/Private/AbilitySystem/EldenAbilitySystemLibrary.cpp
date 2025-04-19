// luce copyright


#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Components/PawnCombatComponent.h"
#include "GameMode/EldenGameMode.h"
#include "HUD/EldenHUD.h"
#include "Interface/CombatInterface.h"
#include "Interface/IPawnCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EldenPlayerState.h"
#include "UI/Controller/EldenWidgetController.h"

bool UEldenAbilitySystemLibrary::GetImpactPointFromEventData(const FGameplayEventData& EventData, FVector& OutImpactPoint)
{
	if (EventData.TargetData.Num() > 0)
	{
		for (const TSharedPtr<FGameplayAbilityTargetData>& TargetData : EventData.TargetData.Data)
		{
			if (TargetData.IsValid())
			{
				// Check for LocationInfo
				if (TargetData->GetScriptStruct() == FGameplayAbilityTargetData_LocationInfo::StaticStruct())
				{
					const FGameplayAbilityTargetData_LocationInfo* LocationData = static_cast<const FGameplayAbilityTargetData_LocationInfo*>(TargetData.Get());
					OutImpactPoint = LocationData->TargetLocation.LiteralTransform.GetLocation();
					UE_LOG(LogTemp, Log, TEXT("ImpactPoint from LocationData: %s"), *OutImpactPoint.ToString());
					return true;
				}
				// Check for HitResult
				else if (TargetData->HasHitResult())
				{
					OutImpactPoint = TargetData->GetHitResult()->ImpactPoint;
					UE_LOG(LogTemp, Log, TEXT("ImpactPoint from HitResult: %s"), *OutImpactPoint.ToString());
					return true;
				}
				// Check for Origin
				else if (TargetData->HasOrigin())
				{
					OutImpactPoint = TargetData->GetOrigin().GetLocation();
					UE_LOG(LogTemp, Log, TEXT("ImpactPoint from Origin: %s"), *OutImpactPoint.ToString());
					return true;
				}
			}
		}
	}

	OutImpactPoint = FVector::ZeroVector;
	return false;
}

UOverlayEldenWidgetController* UEldenAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AEldenHUD* EldenHUD = Cast<AEldenHUD>(PC->GetHUD()))
		{
			AEldenPlayerState* PS = PC->GetPlayerState<AEldenPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return EldenHUD->GetOverlayEldenWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UEldenAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AEldenHUD* EldenHUD = Cast<AEldenHUD>(PC->GetHUD()))
		{
			AEldenPlayerState* PS = PC->GetPlayerState<AEldenPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return EldenHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UEldenAbilitySystemLibrary::InitializeDefaultAttribute(const UObject* WorldContextObject,
                                                            ECharacterClass CharacterClass, float Level,
                                                            UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	FCharacterClassDefaultInfo ClassDefaultInfo = GetCharacterClassInfo(WorldContextObject)->
		GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryEffectContextHandle = ASC->MakeEffectContext();
	PrimaryEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttribute, Level, PrimaryEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryEffectContextHandle = ASC->MakeEffectContext();
	SecondaryEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.SecondaryAttribute, Level, SecondaryEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalEffectContextHandle = ASC->MakeEffectContext();
	VitalEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.VitalAttribute, Level, VitalEffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UEldenAbilitySystemLibrary::GiveStartupAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
                                                    ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbility)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());
		if (CombatInterface)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UEldenAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AEldenGameMode* EldenGameMode = Cast<AEldenGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (EldenGameMode == nullptr)return nullptr;
	return EldenGameMode->CharacterClassInfo;
}

void UEldenAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors,
                                                           const TArray<AActor*>& ActorToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorToIgnore);

	TArray<FOverlapResult> OverlapResults;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(OverlapResults, SphereOrigin, FQuat::Identity,
		                                FCollisionObjectQueryParams(
			                                FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), SphereParams);

		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			if (
				OverlapResult.GetActor()->Implements<UCombatInterface>() &&
				!ICombatInterface::Execute_IsDead(OverlapResult.GetActor())
			)
			{
				OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			}
		}
	}
}

bool UEldenAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

UEldenAbilitySystemComponent* UEldenAbilitySystemLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UEldenAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));

}

UEldenAbilitySystemComponent* UEldenAbilitySystemLibrary::NativeGetEldenASCFromActor(AActor* InActor)
{   
	check(InActor);

	return CastChecked<UEldenAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UEldenAbilitySystemLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UEldenAbilitySystemComponent* ASC = NativeGetEldenASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UEldenAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UEldenAbilitySystemComponent* ASC = NativeGetEldenASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UEldenAbilitySystemLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UEldenAbilitySystemComponent* ASC = NativeGetEldenASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

bool UEldenAbilitySystemLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{   
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(),InDefender->GetActorForwardVector());

	const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"),DotResult,DotResult<-0.1f? TEXT("Valid Block") : TEXT("InvalidBlock"));
 
	 // Debug::Print(DebugString,DotResult<-0.1f? FColor::Green : FColor::Red);

	return DotResult<-0.1f;
}

bool UEldenAbilitySystemLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{   
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

UPawnCombatComponent* UEldenAbilitySystemLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{   
	check(InActor);

	if (IIPawnCombatInterface* PawnCombatInterface = Cast<IIPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UEldenAbilitySystemLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent? EWarriorValidType::Valid : EWarriorValidType::Invalid;
	
	return CombatComponent;
}

bool UEldenAbilitySystemLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UEldenAbilitySystemComponent* SourceASC = NativeGetWarriorASCFromActor(InInstigator);
	UEldenAbilitySystemComponent* TargetASC = NativeGetWarriorASCFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data,TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

void UEldenAbilitySystemLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
                                                     EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor,TagToCheck)? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}
