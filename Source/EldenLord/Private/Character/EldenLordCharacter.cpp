// luce copyright


#include "Character/EldenLordCharacter.h"

#include "AbilitySystemComponent.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/EldenHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/EldenController.h"
#include "Player/EldenPlayerState.h"
#include "Components/EldenCombatComponent.h"
#include "Components/SphereComponent.h"

AEldenLordCharacter::AEldenLordCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	SpringArm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm1->SetupAttachment(GetRootComponent());
	Camera1 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera1->SetupAttachment(SpringArm1);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// EldenCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
	// EldenCollisionSphere->SetupAttachment(GetRootComponent());
	// EldenCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// EldenCollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	// EldenCollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	// EldenCollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// EldenCollisionSphere->SetGenerateOverlapEvents(true);
	// EldenCollisionSphere->InitSphereRadius(100.f);
	// EldenCollisionSphere->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnEldenCollisionSphereBeginOverlap);

	
	EldenCombatComponent = CreateDefaultSubobject<UEldenCombatComponent>(TEXT("EldenCombatComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEldenLordCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEldenLordCharacter::InitAbilityActorInfo()
{
	AEldenPlayerState* EldenPlayerState = GetPlayerState<AEldenPlayerState>();
	check(EldenPlayerState);
	EldenPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(EldenPlayerState, this);
	Cast<UEldenAbilitySystemComponent>(EldenPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = EldenPlayerState->GetAbilitySystemComponent();
	AttributeSet = EldenPlayerState->GetAttributeSet();

	if (AEldenController* EldenController = Cast<AEldenController>(GetController()))
	{
		if (AEldenHUD* EldenHUD = Cast<AEldenHUD>(EldenController->GetHUD()))
		{
			 EldenHUD->InitOverlay(EldenController, EldenPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttribute();
}

void AEldenLordCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AEldenLordCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the server
	InitAbilityActorInfo();
}

void AEldenLordCharacter::OnEldenCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                             AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                             int32 OtherBodyIndex, bool bFromSweep,
                                                             const FHitResult& SweepResult)
{
	Debug::Print(TEXT("ELden Activate"));
	UEldenAbilitySystemComponent* EldenASC = Cast<UEldenAbilitySystemComponent>(
		GetAbilitySystemComponent());
	if (EldenASC)
	{
		EldenASC->TryActivateAbilityByTag(FEldenGameplayTags::Get().Abilities_EldenAssassination);
	}
}

int32 AEldenLordCharacter::GetPlayerLevel()
{
	const AEldenPlayerState* EldenPlayerState = GetPlayerState<AEldenPlayerState>();
	check(EldenPlayerState);
	return EldenPlayerState->GetPlayerLevel();
}

UPawnCombatComponent* AEldenLordCharacter::GetPawnCombatComponent() const
{
	return EldenCombatComponent;
}
