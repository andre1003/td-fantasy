// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseSkill.h"
#include "BaseEnemy.h"
#include "BasePlayableCharacter.h"
#include "DamageSystem.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseSkill::ABaseSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup begin overlap
	OnActorBeginOverlap.AddDynamic(this, &ABaseSkill::OverlapBegin);
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion

#pragma region Overlap
void ABaseSkill::OverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// If skill type is conjuration, exit
	if(SkillType == ESkillType::EST_Conjuration)
	{
		return;
	}

	// Cast other actor to base enemy
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);

	// If cast fail, exit
	if(!Enemy)
	{
		return;
	}

	// Get player character
	ABasePlayableCharacter* Player = Cast<ABasePlayableCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Damage enemy
	Enemy->TakeHit(Player->DamageSystem);
}
#pragma endregion
#pragma endregion

#pragma region Methods

#pragma endregion
