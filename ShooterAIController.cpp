// Copyright Arnold Lam 2020


#include "ShooterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // 0th player

	//SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);

	//MoveToActor(PlayerPawn, 200);

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // 0th player
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // 0th player

	//if (LineOfSightTo(PlayerPawn))
	//{
	//	SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);
	//	MoveToActor(PlayerPawn, AcceptanceRadius);
	//}
	//else
	//{
	//	ClearFocus(EAIFocusPriority::Gameplay);
	//	StopMovement();
	//}

}