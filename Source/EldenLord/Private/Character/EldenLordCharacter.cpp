// luce copyright


#include "Character/EldenLordCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/EldenHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/EldenController.h"
#include "Player/EldenPlayerState.h"
#include "Components/EldenCombatComponent.h"

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
