// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInventorySlot.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseInventorySlot : public AActor
{
	GENERATED_BODY()


#pragma region Attributes
	#pragma region Public Attributes
public:
	// Root scene component
	class USceneComponent* RootSceneComponent;
	#pragma endregion

	#pragma region Protected Attributes
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseItem> Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Amount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsEmpty = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ABaseInventory* Inventory;
	#pragma endregion

	#pragma region Private Attributes

	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABaseInventorySlot();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/// <summary>
	/// Get slot item.
	/// </summary>
	/// <returns>Item class reference.</returns>
	TSubclassOf<class ABaseItem> GetItem();

	/// <summary>
	/// Get slot amount.
	/// </summary>
	/// <returns>Slot amount value.</returns>
	int GetAmount();

	/// <summary>
	/// Increase item amount.
	/// </summary>
	/// <param name="AmountToAdd">- Amount to add.</param>
	void IncreaseAmount(int AmountToAdd);

	/// <summary>
	/// Decrease item amount.
	/// </summary>
	/// <param name="AmountToDecrease">- Amount to decrease.</param>
	void DecreaseAmount(int AmountToDecrease);

	/// <summary>
	/// Add an item to slot.
	/// </summary>
	/// <param name="ItemReference">- Item reference.</param>
	/// <param name="ItemAmount">- Item amount.</param>
	/// <param name="InventoryReference">- Inventory object reference.</param>
	void Add(TSubclassOf<class ABaseItem> ItemReference, int ItemAmount, class ABaseInventory* InventoryReference);

	/// <summary>
	/// Remove the slot from inventory.
	/// </summary>
	void Remove();
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
