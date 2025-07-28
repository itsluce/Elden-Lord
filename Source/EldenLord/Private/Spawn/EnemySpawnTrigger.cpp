// luce copyright

#include "Spawn/EnemySpawnTrigger.h"
#include "Enemy/Enemy.h"
#include "Character/EldenLordCharacter.h"
#include "Interface/CombatInterface.h"
#include "Interface/EnemyInterface.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"

AEnemySpawnTrigger::AEnemySpawnTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create trigger box
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);

	// Create audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetUISound(false); // Enable 3D spatial audio
	AudioComponent->bIsMusic = true; // Mark as music for audio mixing

	// Create debug mesh for editor visualization
	DebugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebugMesh"));
	DebugMesh->SetupAttachment(RootComponent);
	DebugMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DebugMesh->SetHiddenInGame(true);

	// Default values
	SpawnedEnemy = nullptr;
	bHasTriggered = false;
	bEnemySpawned = false;
}

void AEnemySpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind overlap event
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnTrigger::OnTriggerBeginOverlap);
	}
}

void AEnemySpawnTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasTriggered || !CanTrigger(OtherActor))
	{
		return;
	}

	SpawnEnemy();
}

bool AEnemySpawnTrigger::CanTrigger(AActor* TriggeringActor) const
{
	if (!TriggeringActor || !EnemyClassToSpawn)
	{
		return false;
	}

	// Check if player only and verify it's a player character
	if (bPlayerOnly)
	{
		return Cast<AEldenLordCharacter>(TriggeringActor) != nullptr;
	}

	// Allow any pawn if not player only
	return Cast<APawn>(TriggeringActor) != nullptr;
}

void AEnemySpawnTrigger::SpawnEnemy()
{
	if (bHasTriggered || !EnemyClassToSpawn)
	{
		return;
	}

	// Mark as triggered
	bHasTriggered = true;

	// Calculate spawn transform
	FVector TriggerLocation = GetActorLocation();
	FVector SpawnLocation = TriggerLocation + SpawnLocationOffset;
	FRotator SpawnRotation = GetActorRotation() + SpawnRotationOffset;
	
	// Perform a ground trace to find proper spawn height
	FHitResult GroundHit;
	FVector TraceStart = FVector(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z + 1000.0f);
	FVector TraceEnd = FVector(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z - 1000.0f);
	
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = false;
	TraceParams.AddIgnoredActor(this);
	
	if (GetWorld()->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_WorldStatic, TraceParams))
	{
		SpawnLocation.Z = GroundHit.Location.Z + 100.0f; // Spawn 100 units above ground
	}
	
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	// Spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the enemy
	SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClassToSpawn, SpawnTransform, SpawnParams);
	
	if (SpawnedEnemy)
	{
		bEnemySpawned = true;
		
		// Get player for arena creation and AI targeting
		AEldenLordCharacter* Player = Cast<AEldenLordCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		
		// Create arena walls around the player
		if (bCreateArenaWalls && Player)
		{
			// Debug player collision settings
			CreateArenaWalls(Player->GetActorLocation());
		}
		
		// Ensure enemy has proper AI Controller
		if (!SpawnedEnemy->GetController())
		{
			SpawnedEnemy->SpawnDefaultController();
		}
		
		// Initialize the enemy's combat target (set player as target)
		if (Player && SpawnedEnemy->Implements<UEnemyInterface>())
		{
			IEnemyInterface::Execute_SetCombatTarget(SpawnedEnemy, Player);
		}

		// Always monitor enemy death for music cleanup (regardless of bRemoveOnEnemyDeath)
		if (SpawnedEnemy->Implements<UEnemyInterface>() || SpawnedEnemy->Implements<UCombatInterface>())
		{
			// Delay the first death check to allow enemy to fully initialize
			FTimerDelegate DeathCheckDelegate;
			DeathCheckDelegate.BindUObject(this, &AEnemySpawnTrigger::OnEnemyDeath);
			
			GetWorld()->GetTimerManager().SetTimer(
				DeathCheckTimerHandle,
				DeathCheckDelegate,
				2.0f, // Start checking after 2 seconds to let enemy initialize
				true  // Then check every 2 seconds
			);
		}

		// Play boss fight music with persistent component
		if (BossFightMusic)
		{
			// Create a persistent audio component attached to the world
			PersistentMusicComponent = UGameplayStatics::SpawnSoundAtLocation(
				GetWorld(), 
				BossFightMusic, 
				GetActorLocation(), 
				FRotator::ZeroRotator, 
				MusicVolume
			);
			
			if (PersistentMusicComponent && bLoopMusic)
			{
				PersistentMusicComponent->bIsUISound = true; // Make it 2D
				PersistentMusicComponent->bIsMusic = true;
			}
		}

		// Destroy trigger if requested
		if (bDestroyTriggerAfterSpawn)
		{
			// Delay destruction slightly to ensure audio plays
			FTimerDelegate CleanupDelegate;
			CleanupDelegate.BindUObject(this, &AEnemySpawnTrigger::CleanupTrigger);
			
			GetWorld()->GetTimerManager().SetTimer(
				CleanupTimerHandle,
				CleanupDelegate,
				0.1f,
				false
			);
		}
	}
	else
	{
		bHasTriggered = false; // Reset so they can try again
	}
}

void AEnemySpawnTrigger::OnEnemyDeath()
{
	
	// Check if spawned enemy is still valid and alive
	if (!SpawnedEnemy)
	{
		StopBossFightMusic();
		DestroyArenaWalls();
		GetWorld()->GetTimerManager().ClearTimer(DeathCheckTimerHandle);
		
		if (bRemoveOnEnemyDeath)
		{
			CleanupTrigger();
		}
		return;
	}
	
	if (!IsValid(SpawnedEnemy))
	{
		StopBossFightMusic();
		DestroyArenaWalls();
		GetWorld()->GetTimerManager().ClearTimer(DeathCheckTimerHandle);
		
		if (bRemoveOnEnemyDeath)
		{
			CleanupTrigger();
		}
		return;
	}

	// Check if enemy is dead using the CombatInterface
	if (SpawnedEnemy->Implements<UCombatInterface>())
	{
		bool bIsDead = ICombatInterface::Execute_IsDead(SpawnedEnemy);
		
		if (bIsDead)
		{
			StopBossFightMusic();
			DestroyArenaWalls();
			GetWorld()->GetTimerManager().ClearTimer(DeathCheckTimerHandle);
			
			if (bRemoveOnEnemyDeath)
			{
				CleanupTrigger();
			}
		}
	}
}

void AEnemySpawnTrigger::StopBossFightMusic()
{
	// Stop the persistent music component
	if (PersistentMusicComponent && IsValid(PersistentMusicComponent) && PersistentMusicComponent->IsPlaying())
	{
		PersistentMusicComponent->FadeOut(2.0f, 0.0f); // Fade out over 2 seconds
	}
	
	// Also stop the regular audio component if it exists
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->FadeOut(2.0f, 0.0f);
	}
}

void AEnemySpawnTrigger::CreateArenaWalls(const FVector& PlayerLocation)
{
    if (!GetWorld())
    {
        return;
    }

    // Clear any existing walls
    DestroyArenaWalls();

    // Create a single actor to hold all wall segments
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    AActor* CylinderWallActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    
    if (!CylinderWallActor)
    {
        return;
    }

    // Create root component
    USceneComponent* RootComp = NewObject<USceneComponent>(CylinderWallActor, TEXT("CylinderRoot"));
    CylinderWallActor->SetRootComponent(RootComp);
    RootComp->RegisterComponent();

    // Create many thin wall segments to form a cylinder
    const int32 NumSegments = 64; // More segments = smoother cylinder
    const float AngleStep = 360.0f / NumSegments;
    
    for (int32 i = 0; i < NumSegments; i++)
    {
        float Angle = FMath::DegreesToRadians(AngleStep * i);
        
        // Calculate segment position relative to player location
        FVector SegmentDirection = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
        FVector SegmentLocation = PlayerLocation + (SegmentDirection * ArenaRadius);
        
        // Set the proper height for the segment (center it at ground level + half height)
        SegmentLocation.Z = PlayerLocation.Z + WallHeight / 2.0f;
        
        // Create box component for this segment
        UBoxComponent* WallSegment = NewObject<UBoxComponent>(CylinderWallActor, 
            UBoxComponent::StaticClass(), 
            FName(*FString::Printf(TEXT("WallSegment_%d"), i)));
        
        WallSegment->SetupAttachment(RootComp);
        WallSegment->RegisterComponent();
        
        // Position the segment at world location
        WallSegment->SetWorldLocation(SegmentLocation);
        
        // CRITICAL FIX: Rotate segment to be TANGENT to circle, not radial
        // Add 90 degrees to make the wall perpendicular to the radius
        float TangentAngle = FMath::RadiansToDegrees(Angle) + 90.0f;
        WallSegment->SetRelativeRotation(FRotator(0.0f, TangentAngle, 0.0f));
        
        // Calculate segment dimensions
        float CircumferenceAtRadius = 2.0f * PI * ArenaRadius;
        float SegmentWidth = (CircumferenceAtRadius / NumSegments) * 2.0f; // Increase overlap to 100%
        
        // Set box extent - NOTE THE ORDER!
        FVector SegmentExtent = FVector(
            SegmentWidth / 2.0f,        // X - Width along the arc (was Y before)
            WallThickness / 2.0f,       // Y - Thickness pointing inward (was X before)
            WallHeight / 2.0f           // Z - Height
        );
        
        WallSegment->SetBoxExtent(SegmentExtent);
        
        // Set collision properties
        WallSegment->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        WallSegment->SetCollisionObjectType(ECC_WorldStatic);
        WallSegment->SetCollisionResponseToAllChannels(ECR_Block);
        WallSegment->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        WallSegment->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
        
        // Make sure it blocks pawns and characters
        WallSegment->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
        
        // Make invisible unless debugging
        WallSegment->SetHiddenInGame(!bShowArenaDebug);
        WallSegment->SetVisibility(bShowArenaDebug);
        
        // Debug visualization
        if (bShowArenaDebug && (i % 4 == 0)) // Show every 4th segment to reduce clutter
        {
            DrawDebugBox(GetWorld(), SegmentLocation, SegmentExtent, 
                FRotator(0, TangentAngle, 0).Quaternion(), 
                FColor::Green, false, DebugDisplayTime, 0, 5.0f);
                
            // Also draw a line from player center to this wall segment center
            DrawDebugLine(GetWorld(), PlayerLocation, SegmentLocation, FColor::Cyan, false, DebugDisplayTime, 0, 2.0f);
        }
    }
    
    // Store the cylinder actor
    ArenaWalls.Add(CylinderWallActor);
    
    // Debug visualization - draw circles at top and bottom
    if (bShowArenaDebug)
    {
        // Bottom circle
        DrawDebugCircle(GetWorld(), PlayerLocation, ArenaRadius, 64, FColor::Red, false, 
            DebugDisplayTime, 0, 10.0f, FVector(0, 0, 1), FVector(1, 0, 0));
        
        // Top circle
        FVector TopCircleLocation = PlayerLocation;
        TopCircleLocation.Z += WallHeight;
        DrawDebugCircle(GetWorld(), TopCircleLocation, ArenaRadius, 64, FColor::Red, false, 
            DebugDisplayTime, 0, 10.0f, FVector(0, 0, 1), FVector(1, 0, 0));
        
        // Draw some vertical lines that match the actual wall segment positions
        for (int32 i = 0; i < 8; i++)
        {
            float Angle = FMath::DegreesToRadians(45.0f * i);
            FVector LineDirection = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0) * ArenaRadius;
            FVector LineStart = PlayerLocation + LineDirection;
            FVector LineEnd = LineStart + FVector(0, 0, WallHeight);
            DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Yellow, false, DebugDisplayTime, 0, 5.0f);
            
            // Draw center point marker to match wall segment center
            FVector SegmentCenter = PlayerLocation + LineDirection + FVector(0, 0, WallHeight / 2.0f);
            DrawDebugSphere(GetWorld(), SegmentCenter, 25.0f, 8, FColor::Orange, false, DebugDisplayTime);
            
            // Draw line from player to segment center to verify positioning
            DrawDebugLine(GetWorld(), PlayerLocation + FVector(0, 0, WallHeight / 2.0f), SegmentCenter, FColor::Purple, false, DebugDisplayTime, 0, 3.0f);
        }
        
        // Player position
        DrawDebugSphere(GetWorld(), PlayerLocation, 50.0f, 12, FColor::Blue, false, DebugDisplayTime);
    }
}

void AEnemySpawnTrigger::DestroyArenaWalls()
{
	// Destroy all arena walls
	int32 DestroyedCount = 0;
	for (AActor* Wall : ArenaWalls)
	{
		if (Wall && IsValid(Wall))
		{
			Wall->Destroy();
			DestroyedCount++;
		}
	}
	
	// Clear the array
	ArenaWalls.Empty();
}

void AEnemySpawnTrigger::CleanupTrigger()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	
	if (TriggerBox)
	{
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Destroy();
}

void AEnemySpawnTrigger::ManualTrigger()
{
	if (!bHasTriggered)
	{
		SpawnEnemy();
	}
}

#if WITH_EDITOR
void AEnemySpawnTrigger::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// Update debug mesh scale to match trigger box
	if (DebugMesh && TriggerBox)
	{
		FVector BoxExtent = TriggerBox->GetScaledBoxExtent();
		DebugMesh->SetWorldScale3D(BoxExtent / 50.0f); // Normalize to a reasonable size
	}
}
#endif

