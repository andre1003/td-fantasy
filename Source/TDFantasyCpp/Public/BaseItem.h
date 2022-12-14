// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.

#pragma once

#pragma region Includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data.h"
#include "BaseItem.generated.h"
#pragma endregion


UCLASS()
class TDFANTASYCPP_API ABaseItem : public AActor
{
	GENERATED_BODY()


#pragma region Attributes
	#pragma region Public Attributes
public:
	// Use UI widget component
	UPROPERTY(EditDefaultsOnly)
		class UWidgetComponent* UseUI;
	#pragma endregion

	#pragma region Protected Attributes
protected:
	#pragma region Stats
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		int BuyPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		int SellPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		int DiscoveryXP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		TEnumAsByte<EItemType> ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
		TEnumAsByte<EItemRarity> ItemRarity;
	#pragma endregion

	#pragma region Information
	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Information")
		FText ItemName;

	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Information")
		FText ItemDescription;

	UPROPERTY(EditDefaultsonly, BlueprintReadWrite, Category = "Information")
		class UObject* ItemIcon;
	#pragma endregion
	#pragma endregion

	#pragma region Private Attributes
	// UseUI visibility reset timer
	FTimerHandle ResetTimer;
	#pragma endregion
#pragma endregion


#pragma region Methods
	#pragma region Public Methods
public:
	// Sets default values for this actor's properties
	ABaseItem();

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


	/// <summary>
	/// Use an item.
	/// </summary>
	virtual void UseItem();

	#pragma region Getters
	#pragma region Stats
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get buy price value.
		/// </summary>
		/// <returns>Buy price.</returns>
		int GetBuyPrice();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get sell price value.
		/// </summary>
		/// <returns>Sell price.</returns>
		int GetSellPrice();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get XP when discorvery this item.
		/// </summary>
		/// <returns>Discovery XP.</returns>
		int GetDiscoveryXP();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get item type.
		/// </summary>
		/// <returns>Item type.</returns>
		TEnumAsByte<EItemType> GetItemType();
	
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get item rarity.
		/// </summary>
		/// <returns>Item rarity.</returns>
		TEnumAsByte<EItemRarity> GetItemRarity();
	#pragma endregion

	#pragma region Information
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get item name.
		/// </summary>
		/// <returns>Item name.</returns>
		FText GetItemName();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get item description.
		/// </summary>
		/// <returns>Item description.</returns>
		FText GetItemDescription();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Get item icon.
		/// </summary>
		/// <returns>Item icon.</returns>
		class UObject* GetItemIcon();
	#pragma endregion
	#pragma endregion
	#pragma endregion

	#pragma region Protected Methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	#pragma endregion

	#pragma region Private Methods
	/// <summary>
	/// Reset UseUI visibility to false.
	/// </summary>
	void ResetUseUIVisibility();
	#pragma endregion
#pragma endregion
};
