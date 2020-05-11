#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	UFUNCTION()
	void CheckSpawn();

	void Init( UWorld *world, UStaticMeshComponent *mesh );
	void Flock();

private:
	UWorld *World;	
	bool initialized;
	TArray<class AAgent *> Agents;
	FVector rule1(AAgent* a);
	FVector rule2(AAgent* a);
	FVector rule3(AAgent* a);
	int current_agent_count;
	FTimerHandle RespawnTimerHandle;
	UStaticMeshComponent *AgentMesh;
	int id;
};