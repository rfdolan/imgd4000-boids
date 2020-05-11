// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/InputComponent.h"
#include "ShipPawn.generated.h"

UCLASS()
class FLOCKING_API AShipPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent * Mesh;

public:
	// Sets default values for this pawn's properties
	AShipPawn();

protected:
	UFloatingPawnMovement *PawnMovement;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveUp( float Amount);
	void MoveRight( float Amount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Fire();
	float ProjectileSpeed;

};
