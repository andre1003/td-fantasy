// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BasePotion.h"
#include "TDFantasyCpp/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#pragma endregion


#pragma region Contructor
// Sets default values
ABasePotion::ABasePotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABasePotion::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ABasePotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion

#pragma region Methods
void ABasePotion::UsePotion()
{
	// Get player character
	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Restore stats
	Player->RestoreHealthAndMana(HealthRestore, ManaRestore);
}
#pragma endregion
