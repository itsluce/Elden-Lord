// luce copyright


#include "AbilitySystem/Ability/EldenSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UEldenSummonAbility::GetSummonLocation()
{
    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    if (!AvatarActor)
    {
        return {};
    }

    const FVector Forward = AvatarActor->GetActorRightVector();
    const FVector Location = AvatarActor->GetActorLocation();

    if (NumMinions <= 0 || SpawnSpread <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid NumMinions or SpawnSpread!"));
        return {};
    }

    const float DeltaSpread = SpawnSpread / NumMinions;
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
    TArray<FVector> SpawnLocations;

    for (int32 i = 0; i < NumMinions; i++)
    {
        const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
        FVector ChosenSpawnLocation = Location + Direction *
                                       FMath::FRandRange(FMath::Min(MinSpawnDistance, MaxSpawnDistance),
                                                         FMath::Max(MinSpawnDistance, MaxSpawnDistance));

        FHitResult Hit;
        if (GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0, 0, 400.f),
                                                 ChosenSpawnLocation - FVector(0, 0, 400.f), ECC_Visibility))
        {
            ChosenSpawnLocation = Hit.ImpactPoint;
        }

        if (!ChosenSpawnLocation.ContainsNaN())
        {
            SpawnLocations.Add(ChosenSpawnLocation);

            if (bShowDebug)
            {
                DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);
                UKismetSystemLibrary::DrawDebugArrow(AvatarActor, Location,
                                                     Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);
                DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
                DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("NaN found in ChosenSpawnLocation!"));
        }
    }

    return SpawnLocations;
}

TSubclassOf<APawn> UEldenSummonAbility::GetRandomMinionClass()
{
	int32 Selection = FMath::RandRange(0 , MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
