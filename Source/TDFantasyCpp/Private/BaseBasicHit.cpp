// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseBasicHit.h"
#pragma endregion


#pragma region Contructor
// Sets default values
ABaseBasicHit::ABaseBasicHit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABaseBasicHit::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseBasicHit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion
#pragma endregion
