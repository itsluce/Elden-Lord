// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "EnemySpawnTrigger.generated.h"

class AEnemy;
class USoundBase;
class AEldenLordCharacter;

UCLASS()
class ELDENLORD_API AEnemySpawnTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
AEnemySpawnTrigger();

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DebugMesh;

	// Spawn Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	TSubclassOf<AEnemy> EnemyClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	FVector SpawnLocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	FRotator SpawnRotationOffset = FRotator::ZeroRotator;

	// Audio Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* BossFightMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MusicVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	bool bLoopMusic = true;

	// Trigger Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
	bool bPlayerOnly = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
	bool bDestroyTriggerAfterSpawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
	bool bRemoveOnEnemyDeath = true;

	// Arena Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena Settings")
	bool bCreateArenaWalls = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena Settings")
	float ArenaRadius = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena Settings")
	float WallHeight = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena Settings")
	float WallThickness = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena Settings")
	bool bShowArenaDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena Settings")
	float DebugDisplayTime = 30.0f;

private:
	// Collision Events
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Spawn Logic
	void SpawnEnemy();
	bool CanTrigger(AActor* TriggeringActor) const;

	// Enemy Death Monitoring
	UFUNCTION()
	void OnEnemyDeath();

	// Audio Management
	void StopBossFightMusic();

	// Arena Management
	void CreateArenaWalls(const FVector& PlayerLocation);
	void DestroyArenaWalls();

	// Cleanup
	void CleanupTrigger();

	// State Variables
	UPROPERTY()
	AEnemy* SpawnedEnemy;
	
	bool bHasTriggered = false;
	bool bEnemySpawned = false;

	// Timer Handles
	FTimerHandle DeathCheckTimerHandle;
	FTimerHandle CleanupTimerHandle;
	
	// Music Management
	UPROPERTY()
	UAudioComponent* PersistentMusicComponent;

	// Arena Wall Management
	UPROPERTY()
	TArray<AActor*> ArenaWalls;

public:
	// Blueprint Interface
	UFUNCTION(BlueprintCallable, Category = "Spawn Trigger")
	void ManualTrigger();

	UFUNCTION(BlueprintCallable, Category = "Spawn Trigger")
	bool IsTriggered() const { return bHasTriggered; }

	UFUNCTION(BlueprintCallable, Category = "Spawn Trigger")
	bool HasEnemySpawned() const { return bEnemySpawned; }

	UFUNCTION(BlueprintCallable, Category = "Spawn Trigger")
	AEnemy* GetSpawnedEnemy() const { return SpawnedEnemy; }

	UFUNCTION(BlueprintCallable, Category = "Spawn Trigger")
	void SetEnemyClass(TSubclassOf<AEnemy> NewEnemyClass) { EnemyClassToSpawn = NewEnemyClass; }

	// Editor Helpers
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
