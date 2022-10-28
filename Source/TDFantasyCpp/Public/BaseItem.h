// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class TDFANTASYCPP_API ABaseItem : public AActor
{
	GENERATED_BODY()


#pragma region Attributes
	#pragma region Public Attributes

	#pragma endregion

	#pragma region Protected Attributes
protected:
	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Information")
		FText ItemName;

	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Information")
		class UObject* ItemIcon;
	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABaseItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FText GetItemName();

	class UObject* GetItemIcon();
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
