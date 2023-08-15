// Lincensed under the WTFPL v2


#include "BoidActor.h"
#include "BoidSubsystem.h"

// Sets default values
ABoidActor::ABoidActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Values
	MaxSpeed = 1200.f;
	SeparationThreshold = 300.f;
	SeparationTerm = 1.f;
	AlignmentThreshold = 600.f;
	AlignmentTerm = 1.f;
	CohesionThreshold = 800.f;
	CohesionTerm = 5.f;
	AvoidanceThreshold = 400.f;
	AvoidanceTerm = 20.f;
	VarianceTerm = 100.f;
}

// Called when the game starts or when spawned
void ABoidActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<UBoidSubsystem>()->AddBoid(this);
}

// Called every frame
void ABoidActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAppearance(DeltaTime);
}

void ABoidActor::FaceVelocity(const float RollSpringCoefficient)
{
	SetActorRotation(FQuat::FindBetween(FVector::ForwardVector, Velocity));
}
