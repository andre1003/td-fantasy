// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePotion.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABasePotion : public AActor
{
	GENERATED_BODY()
	
#pragma region Attributes
	#pragma region Public Attributes

	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region Restore
	// Health restore
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Restore")
		int HealthRestore;

	// Mana restore
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Restore")
		int ManaRestore;
	#pragma endregion

	#pragma region Price
	// Price
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Price")
		int BuyPrice;
	#pragma endregion

	#pragma region Cooldown
	// Potion reuse cooldown (in seconds)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cooldown")
		float Cooldown = 3.f;

	// Can use potion controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cooldown")
		bool bCanUsePotion = true;

	// Cooldown timer
	FTimerHandle CooldownTimer;
	#pragma endregion
	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABasePotion();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	/// <summary>
	/// Use this potion.
	/// </summary>
	void UsePotion();

	/// <summary>
	/// Use this potion C++ implementation.
	/// </summary>
	void UsePotion_Implementation();
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods
private:
	void ResetCooldown();
	#pragma endregion
#pragma endregion
};
