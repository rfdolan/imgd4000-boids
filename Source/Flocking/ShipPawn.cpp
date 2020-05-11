// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPawn.h"
#include "DrawDebugHelpers.h"
#include "ProjectileActor.h"
#include "Agent.h"
#include "GameFramework/CharacterMovementComponent.h"

AShipPawn::AShipPawn(){
	PrimaryActorTick.bCanEverTick = true;
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ProjectileSpeed = 10.f;
}
// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShipPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveUp", this, &AShipPawn::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShipPawn::MoveRight);	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShipPawn::Fire );

}


void AShipPawn::MoveUp( float Amount ) {
	PawnMovement->AddInputVector( GetActorUpVector() * Amount );
}

void AShipPawn::MoveRight( float Amount ) {
	PawnMovement->AddInputVector( GetActorRightVector() * Amount );
}

void AShipPawn::Fire() {
	FVector start = GetActorLocation();
	//start.Z += 100.f;
	//start.Y += 100.f;
	start.X += 150.f;
	float distance = 1000.f;
	FVector fv = GetActorForwardVector();
	FVector end = ((fv * distance) + start);
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.f, 0, 5.f);

	FCollisionQueryParams cqp;
	FHitResult hr;
	GetWorld()->LineTraceSingleByChannel(hr, start, end, ECC_Visibility, cqp);
	if( hr.bBlockingHit == true ) {	
		if( hr.GetActor() != this && hr.GetActor()->ActorHasTag("Agent")) {
			UE_LOG(LogTemp, Warning, TEXT("HIT! %s"), *hr.GetActor()->GetName() );
			AAgent *hit = Cast<AAgent>(hr.GetActor());
			hit->MarkForDelete();
		}
	}
}