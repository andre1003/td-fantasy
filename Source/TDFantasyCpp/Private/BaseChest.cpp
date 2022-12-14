// This code was developed by Paulo André "Dufwine" Pimenta Aragão. All rights reserved.


#pragma region Includes
#include "BaseChest.h"
#include "BaseInventory.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/OutputDeviceNull.h"
#include "TDFantasyCpp/BaseTopDownCharacter.h"
#pragma endregion


#pragma region Constructor
// Sets default values
ABaseChest::ABaseChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = RootSceneComponent;

	// Setup UseUI widget component
	UseUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("UseUI"));
	UseUI->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	UseUI->SetWidgetSpace(EWidgetSpace::Screen);
	UseUI->SetVisibility(false);
	UseUI->SetupAttachment(RootComponent);

	// Setup SphereCollision component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereCollision->SetSphereRadius(200.f);
	SphereCollision->SetupAttachment(RootComponent);

	// Overlap methods binding
	OnActorBeginOverlap.AddDynamic(this, &ABaseChest::OverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ABaseChest::OverlapEnd);
}
#pragma endregion

#pragma region Events
#pragma region Begin Play
// Called when the game starts or when spawned
void ABaseChest::BeginPlay()
{
	Super::BeginPlay();

	// Get player inventory
	PlayerInventory = Cast<ABaseInventory>(UGameplayStatics::GetActorOfClass(GetWorld(), InventoryClass));
}

#pragma endregion

#pragma region Tick
// Called every frame
void ABaseChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma endregion

#pragma region Overlap
void ABaseChest::OverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// If chest is open, actor is being destroyed or there is no actor overlapping, exit
	if(bIsOpen || IsActorBeingDestroyed() || !OtherActor)
	{
		return;
	}

	// Cast other actor to base character class
	ABaseTopDownCharacter* Player = Cast<ABaseTopDownCharacter>(OtherActor);

	// If cast fails, exit
	if(!Player)
	{
		return;
	}

	// Set player's temporary chest
	Player->SetTempChest(this);

	// Set UseUI visible
	UseUI->SetVisibility(true);

	// Call ShowUI event of widget
	FOutputDeviceNull OutputDeviceNull;
	UseUI->GetWidget()->CallFunctionByNameWithArguments(TEXT("ShowUI"), OutputDeviceNull, nullptr, true);
}

void ABaseChest::OverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	// If chest is open, actor is being destroyed or there is no actor overlapping, exit
	if(bIsOpen || IsActorBeingDestroyed() || !OtherActor)
	{
		return;
	}

	// Cast other actor to base character class
	ABaseTopDownCharacter* Player = Cast<ABaseTopDownCharacter>(OtherActor);

	// If cast fails, exit
	if(!Player)
	{
		return;
	}

	// Set temporary chest to null
	Player->SetTempChest(nullptr);

	// Call Hide event of widget
	FOutputDeviceNull OutputDeviceNull;
	UseUI->GetWidget()->CallFunctionByNameWithArguments(TEXT("HideUI"), OutputDeviceNull, nullptr, true);

	// After 0.25 seconds, reset UseUI visibility
	GetWorldTimerManager().SetTimer(ResetTimer, this, &ABaseChest::ResetUseUIVisibility, 0.25f);
}
#pragma endregion
#pragma endregion

#pragma region Methods
void ABaseChest::CollectItems()
{
	// Loop reward items
	int ItemsLength = Items.Num();
	for(int i = 0; i < ItemsLength; i++)
	{
		// Add each item to player inventory and remove it from this list
		PlayerInventory->AddItem(Items[i], ItemsAmount[i]);
		Items.RemoveAt(i);
		ItemsAmount.RemoveAt(i);
	}

	// Get player
	ABaseTopDownCharacter* Player = Cast<ABaseTopDownCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Randomize coins and gems and add then to player's wallet
	int CoinsToAdd = UKismetMathLibrary::RandomIntegerInRange(0, MaxCoins);
	int GemsToAdd = UKismetMathLibrary::RandomIntegerInRange(0, MaxGems);
	Player->AddMoney(CoinsToAdd, GemsToAdd);

	// Add XP
	//
}

void ABaseChest::ResetUseUIVisibility()
{
	// Clear reset timer
	GetWorldTimerManager().ClearTimer(ResetTimer);

	// Set UseUI to invisible
	UseUI->SetVisibility(false);
}
#pragma endregion
