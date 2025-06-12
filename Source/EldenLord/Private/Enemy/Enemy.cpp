// luce copyright


#include "Enemy/Enemy.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "EldenGameplayTags.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "AbilitySystem/EldenAttributeSet.h"
#include "AI/EldenAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EldenLordCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/EnemyCombatComponent.h"
#include "EldenLord/EldenLord.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "HUD/EldenOverlay.h"
#include "UI/Widget/EldenUserWidget.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,180.f,0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UEldenAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEldenAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EldenCombatComponent"));
	
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())return;
	EldenAIController = Cast<AEldenAIController>(NewController);
	EldenAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EldenAIController->RunBehaviorTree(BehaviorTree);

	EldenAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	EldenAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),
	                                                            CharacterClass != ECharacterClass::Warrior);
	EldenAIController->GetBlackboardComponent()->SetValueAsBool(FName("ShamanAttacker"),
	                                                            CharacterClass == ECharacterClass::Shaman);
	EldenAIController->GetBlackboardComponent()->SetValueAsBool(FName("InvisibleAttacker"),
	                                                            CharacterClass == ECharacterClass::Invisible);
	EldenAIController->GetBlackboardComponent()->SetValueAsBool(FName("HealerAttacker"),
	                                                            CharacterClass == ECharacterClass::Healer);
	EldenAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DefaultMaxWalkSpeed"),GetCharacterMovement()->MaxWalkSpeed);
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UEldenAbilitySystemLibrary::GiveStartupAbility(this, AbilitySystemComponent, CharacterClass);

	if (UEldenUserWidget* EldenUserWidget = Cast<UEldenUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		EldenUserWidget->SetWidgetController(this);
	}

	if (UEldenAttributeSet* EldenAS = Cast<UEldenAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EldenAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EldenAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FEldenGameplayTags::Get().Effect_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AEnemy::HitReactTagChanged
		);
		OnHealthChanged.Broadcast(EldenAS->GetHealth());
		OnMaxHealthChanged.Broadcast(EldenAS->GetMaxHealth());
	}
}

void AEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReact = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReact ? 0.f : BaseWalkSpeed;

	EldenAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReact);
}

void AEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	UEldenAbilitySystemComponent* EldenASC = Cast<UEldenAbilitySystemComponent>(AbilitySystemComponent);
	EldenASC->TryActivateAbilityByTag(FEldenGameplayTags::Get().Enemy_Ability_SpawnStone);
	Super::Die();
}

void AEnemy::InitializeDefaultAttribute() const
{
	UEldenAbilitySystemLibrary::InitializeDefaultAttribute(this, CharacterClass, Level, AbilitySystemComponent);
}

void AEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(custom_depth_red);
}

void AEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void AEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

UPawnCombatComponent* AEnemy::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}


void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UEldenAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttribute();
}

int32 AEnemy::GetPlayerLevel()
{
	return Level;
}
