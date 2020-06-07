// Copyright Arnold Lam 2020


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates and sets scene component for gun
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Creates and sets mesh component for gun
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	// Muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner()); // gets ShooterCharacter owner. Try to be as non-specific as possible
	if (!OwnerPawn) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("Gun cannot get owner pawn"));
		return; 
	}

	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController) {	
		UE_LOG(LogTemp, Error, TEXT("Gun cannot get controller for owner pawn"));
		return;	
	}

	// Get transform of where camera is looking at
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT Location, OUT Rotation); 

	// Calculate the target of the ray trace
	Location.Z += AimingOffset; // Offset for aiming
	FVector End = Location + Rotation.Vector() * MaxBulletRange;

	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactExplosion, Hit.Location, ShotDirection.Rotation());
		
		if (Hit.GetActor()) 
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			Hit.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}


	}
	
	//DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, true);
}

