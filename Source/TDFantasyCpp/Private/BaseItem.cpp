// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseItem.h"
#include "Components/WidgetComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "TDFantasyCpp/BaseTopDownCharacter.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup UseUI widget component
	UseUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UseUI"));
	UseUI->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	UseUI->SetWidgetSpace(EWidgetSpace::Screen);
	UseUI->SetVisibility(false);
	UseUI->SetupAttachment(RootComponent);

	// Bind overlap events
	OnActorBeginOverlap.AddDynamic(this, &ABaseItem::OverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ABaseItem::OverlapEnd);
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

}
#pragma endregion

#pragma region Tick
// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#pragma endregion

#pragma region Overlap
void ABaseItem::OverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// If actor is being destroyed or there is no actor overlapping this item, exit
	if(IsActorBeingDestroyed() || !OtherActor)
	{
		return;
	}

	// Try cast the overlapping actor
	ABaseTopDownCharacter* Player = Cast<ABaseTopDownCharacter>(OtherActor);

	// If cast fails, exit
	if(!Player)
	{
		return;
	}

	// Set temporary item to this
	Player->SetTempItem(this);

	// Set UseUI visibility to true
	UseUI->SetVisibility(true);

	// Call ShowUI event of widget
	FOutputDeviceNull OutputDeviceNull;
	UseUI->GetWidget()->CallFunctionByNameWithArguments(TEXT("ShowUI"), OutputDeviceNull, nullptr, true);
}

void ABaseItem::OverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	// If actor is being destroyed or there is no actor overlapping this item, exit
	if(IsActorBeingDestroyed() || !OtherActor)
	{
		return;
	}

	// Try cast the overlapping actor
	ABaseTopDownCharacter* Player = Cast<ABaseTopDownCharacter>(OtherActor);

	// If cast fails, exit
	if(!Player)
	{
		return;
	}

	// Set temporary item to null
	Player->SetTempItem(nullptr);

	// Call Hide event of widget
	FOutputDeviceNull OutputDeviceNull;
	UseUI->GetWidget()->CallFunctionByNameWithArguments(TEXT("HideUI"), OutputDeviceNull, nullptr, true);

	// After 0.25 seconds, reset UseUI visibility
	GetWorldTimerManager().SetTimer(ResetTimer, this, &ABaseItem::ResetUseUIVisibility, 0.25f);
}
#pragma endregion
#pragma endregion

#pragma region Methods
void ABaseItem::ResetUseUIVisibility()
{
	// Clear reset timer
	GetWorldTimerManager().ClearTimer(ResetTimer);

	// Set UseUI visibility to false
	UseUI->SetVisibility(false);
}

void ABaseItem::UseItem()
{
	// This method must be override to work
}
#pragma endregion

#pragma region Getters
#pragma region Stats
int ABaseItem::GetBuyPrice()
{
	return BuyPrice;
}

int ABaseItem::GetSellPrice()
{
	return SellPrice;
}

int ABaseItem::GetDiscoveryXP()
{
	return DiscoveryXP;
}

TEnumAsByte<EItemType> ABaseItem::GetItemType()
{
	return ItemType;
}

TEnumAsByte<EItemRarity> ABaseItem::GetItemRarity()
{
	return ItemRarity;
}
#pragma endregion

#pragma region Information
FText ABaseItem::GetItemName()
{
	return ItemName;
}

FText ABaseItem::GetItemDescription()
{
	return ItemDescription;
}

UObject* ABaseItem::GetItemIcon()
{
	return nullptr;
}
#pragma endregion
#pragma endregion
