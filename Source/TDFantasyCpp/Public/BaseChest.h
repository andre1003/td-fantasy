// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Include
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseChest.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseChest : public AActor
{
	GENERATED_BODY()
	

#pragma region Attributes
	#pragma region Public Attributes

	#pragma endregion

	#pragma region Protected Attributes

	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABaseChest();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods

	#pragma endregion
#pragma endregion
};
