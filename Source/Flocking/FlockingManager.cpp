#include "FlockingManager.h"

#define AGENT_COUNT 10
#define MAX_SPEED 10
#define MAX_AGENTS 20
#define RESPAWN_RATE 5
#define MAX_SPAWN_DIST 500

void UFlockingManager::Init( UWorld *world, UStaticMeshComponent *mesh ) {
    UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));
    AgentMesh = mesh;
    id = 0;
    
    current_agent_count = AGENT_COUNT;
    World = world;
    
    float incr = (PI * 2.f) / current_agent_count;
    for( int i = 0; i < current_agent_count; i++ ) {
        if( World != nullptr ) {
            FRotator rotation = FRotator();

            FVector location = FVector(0.f);
            location.X = FMath::Sin( incr * i ) * 150.f;
            location.Y = FMath::Cos( incr * i ) * 150.f;
            location.Z = FMath::RandRange(-5.0f, 5.0f);

            AAgent * agent = World->SpawnActor<AAgent>( location, rotation );
            agent->Init( mesh, id );
            id++;
            Agents.Add( agent );
        }
    }
    World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UFlockingManager::CheckSpawn, RESPAWN_RATE, false);

    initialized = true;
}

void UFlockingManager::Flock() {
    

    for(int agentnum = 0;agentnum < Agents.Num(); agentnum++) {
        AAgent *A = Agents[agentnum];
        if(A->ShouldDelete()) {
            Agents.Remove(A);
            A->Destroy();
            UE_LOG(LogTemp, Warning, TEXT("Deleted agent!"));
        }
    }

    FVector v1, v2, v3;
    AAgent* A;
    for(int i=0; i<Agents.Num(); i++) {

        A = Agents[i];
        v1 = rule1(A);
        v2 = rule2(A);
        v3 = rule3(A);

        // set a's velocity
        A->Velocity = A->Velocity + v1 +v2 ;//+ v3;
        if(A->Velocity.Size() > MAX_SPEED) {
            FVector vel = A->Velocity;
            vel.Normalize();
            A->Velocity = vel * MAX_SPEED;
        }
        // set a's position
        A->GetRootComponent()->SetWorldLocation(A->GetActorLocation() + A->Velocity);
        FVector dist_from_spawn = FVector(0.f) - A->GetActorLocation();
        if(dist_from_spawn.Size() > MAX_SPAWN_DIST) {
            dist_from_spawn.Normalize();
            A->GetRootComponent()->SetWorldLocation(dist_from_spawn * MAX_SPAWN_DIST);
        }
    }

}

// Fly towards center of mass
FVector UFlockingManager::rule1(AAgent* agent) {
    FVector center = FVector(0.f);
    AAgent* curr;
    for(int i=0; i<Agents.Num(); i++) {
        curr = Agents[i];
        if(curr->GetUniqueID() != agent->GetUniqueID()) {
            center = center + curr->GetActorLocation();
        }
    }
    center = center / (Agents.Num() -1);
    return (center - agent->GetActorLocation()) / 100;
}

// Keep small distance
FVector UFlockingManager::rule2(AAgent* agent) {
    FVector c = FVector(0.f);
    AAgent* curr;
    for(int i=0; i<Agents.Num(); i++) {
        curr = Agents[i];
        if(curr->GetUniqueID() != agent->GetUniqueID()) {
            if((curr->GetActorLocation() - agent->GetActorLocation()).Size() < 100) {
                c = c - (curr->GetActorLocation() - agent->GetActorLocation());
            }
        }
    }
    return c;
}

// Match velocity
FVector UFlockingManager::rule3(AAgent* agent) {
    
    FVector vel = FVector(0.f);
    
    AAgent* curr;
    for(int i=0; i<Agents.Num(); i++) {
        curr = Agents[i];
        if(curr->GetUniqueID() != agent->GetUniqueID()) {
            vel = vel + curr->Velocity;
        }
    }
    vel = vel / (Agents.Num() - 1);

    return vel;

}

void UFlockingManager::CheckSpawn() {
    UE_LOG(LogTemp, Warning, TEXT("Spawn!"));
    World->GetTimerManager().ClearTimer(RespawnTimerHandle);
    if(Agents.Num() < MAX_AGENTS) {
        if( World != nullptr ) {
            FRotator rotation = FRotator();


            FVector location = FVector(0.f);
            location.Y = FMath::RandRange(-5.0f, 5.0f);
            location.Z = FMath::RandRange(-5.0f, 5.0f);

            AAgent * agent = World->SpawnActor<AAgent>( location, rotation );
            agent->Init( AgentMesh, id );
            id++;
            Agents.Add( agent );
            current_agent_count++;
            UE_LOG(LogTemp, Warning, TEXT("Spawned agent!"));
        }
    }

    World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UFlockingManager::CheckSpawn, RESPAWN_RATE, false);

}
