#include "Agent.h"
#include "Kismet/GameplayStatics.h"
#include "FlockingGameModeBase.h"

AAgent::AAgent(){
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AgentMesh"));	
	RootComponent = Mesh;
	Velocity = FVector(0.f);
	Tags.Add("Agent");
	ToDelete = false;
}

void AAgent::BeginPlay(){
	Super::BeginPlay();
}

void AAgent::Init( UStaticMeshComponent *mesh, int id ) {
	UE_LOG(LogTemp, Warning, TEXT("Agent initialized.") );
	Mesh->SetStaticMesh( mesh->GetStaticMesh() );
}

void AAgent::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	FVector loc = GetActorLocation();
	SetActorLocation( loc + Velocity );
}

void AAgent::MarkForDelete() {
	UE_LOG(LogTemp,Warning,TEXT("NOT FLOATING"));
	ToDelete = true;
}

bool AAgent::ShouldDelete() {
	return ToDelete;
}