// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseBasicHit.h"
#include "BaseEnemy.h"
#include "BasePlayableCharacter.h"
#include "DamageSystem.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Contructor
// Sets default values
ABaseBasicHit::ABaseBasicHit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	// Overlap binding setup
	OnActorBeginOverlap.AddDynamic(this, &ABaseBasicHit::OverlapBegin);
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABaseBasicHit::BeginPlay()
{
	Super::BeginPlay();

	// Basic hit light flickering
	LightFlickerLoop();

	// Get player character and focus enemy
	ABasePlayableCharacter* Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ABaseEnemy* FocusEnemy = Player->GetFocusEnemy();

	// If there is no focus enemy, destroy this actor
	if(!FocusEnemy)
	{
		Destroy();
		return;
	}

	// Move basic hit to enemy in 0.2 seconds
	FVector TargetLocation = FocusEnemy->GetActorLocation();
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, TargetLocation, FRotator::ZeroRotator, false, false,
		0.2f, false, EMoveComponentAction::Type::Move, LatentInfo);

	// Set life span to 5 seconds
	SetLifeSpan(5.f);
}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseBasicHit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion

#pragma region Overlap
void ABaseBasicHit::OverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// If actor is being destroyed or there is no other actor, exit
	if(IsActorBeingDestroyed() || !OtherActor)
	{
		return;
	}

	// Cast other actor to enemy
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);

	// If cast fails or enemy is dead, exit
	if(!Enemy || Enemy->GetIsDead())
	{
		return;
	}

	// Get player character and attack enemy (since it is a basic hit, no AP damage is done)
	ABasePlayableCharacter* Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Enemy->TakeHit(Player->DamageSystem);

	// Destroy actor
	Destroy();
}
#pragma endregion
#pragma endregion
