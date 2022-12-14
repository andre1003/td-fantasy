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
public:
	// Root scene component
	class USceneComponent* RootSceneComponent;

	// Sphere collision
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* SphereCollision;

	// Use UI widget component
	UPROPERTY(EditDefaultsOnly)
		class UWidgetComponent* UseUI;
	#pragma endregion

	#pragma region Protected Attributes
protected:
	// Reward items array
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		TArray<TSubclassOf<class ABaseItem>> Items;

	// Reward items amount array (the ItemAmount[Index] matches the Items[Index])
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		TArray<int> ItemsAmount;


	// Inventory base class reference
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
		TSubclassOf<class ABaseInventory> InventoryClass;


	// Maximum reward coins
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		int MaxCoins;

	// Maximum reward gems
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		int MaxGems;

	// Reward XP
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
		int XP;


	// Is chest open controller
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
		bool bIsOpen = false;
	#pragma endregion

	#pragma region Private Attributes
	// Player's inventory reference
	class ABaseInventory* PlayerInventory;

	// Reset timer
	FTimerHandle ResetTimer;
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABaseChest();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		/// <summary>
		/// Begin overlap override method.
		/// </summary>
		void OverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		/// <summary>
		/// End overlap override method.
		/// </summary>
		void OverlapEnd(AActor* OverlappedActor, AActor* OtherActor);


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		/// <summary>
		/// Open this chest.
		/// </summary>
		void OpenChest();
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Collect items and give then to the player.
		/// </summary>
		void CollectItems();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Reset UseUI visibility to false.
		/// </summary>
		void ResetUseUIVisibility();
	#pragma endregion
#pragma endregion
};
