// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	// Set gun
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this); // Set character as owner of gun (relative for damage and multiplayer)
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IsDead();
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpRate); // for gamepad
	PlayerInputComponent->BindAxis("LookRight", this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAxis("LookRightRate", this, &AShooterCharacter::LookRightRate); // for gamepad
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	// Go forward / backword. Axis Value is 1 or -1
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::Jump()
{
	ACharacter::Jump();
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	// Delta seconds is the amount of second from the last frame
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	// Delta seconds is the amount of second from the last frame
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageApplied = FMath::Min(Health, DamageApplied);

	Health = Health - DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Enemy health has been reduced to %f"), Health);

	return DamageApplied;
}

bool AShooterCharacter::IsDead() const
{
	if (Health <= 0)
	{
		return true;
	}

	return false;
}

