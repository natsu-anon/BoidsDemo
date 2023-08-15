// Lincensed under the WTFPL v2

#include "BoidSubsystem.h"
#include "BoidSubsystem.h"
#include "Octree.h"
#include "BoidActor.h"

#define OCTREE_SPLIT_NUM 20 // idk just feels appropriate
#define OCTREE_MIN_DIM 800.f // use the largest boid threshold value to determine this value
#define FLOOR_HEIGHT 200.f

void UBoidSubsystem::Initialize(FSubsystemCollectionBase& Collections)
{
	Super::Initialize(Collections);
	bTickable = false;
	BoidOctree = new FOctree(OCTREE_SPLIT_NUM, OCTREE_MIN_DIM);
}

void UBoidSubsystem::Deinitialize()
{
	Super::Deinitialize();
	Boids.Empty();
	delete BoidOctree;
}

TStatId UBoidSubsystem::GetStatId() const
{
	return TStatId();
}

void UBoidSubsystem::AddBoid(ABoidActor* Boid)
{
	Boids.Add(Boid);
	BoidOctree->Add(Boid);
	bTickable = true;
}

void UBoidSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bTickable)
	{
		return;
	}
	BoidOctree->EmptyChildren();
	BoidOctree->Build();
	ABoidActor* Boid;
	FVector Pos;
	for (int i = 0; i < BoidOctree->Boids.Num(); i++)
	{
		Boid = BoidOctree->Boids[i];
		Pos = Boid->GetActorLocation();
		TickBoid(DeltaTime, Boid, Pos);
	}
}

void UBoidSubsystem::TickBoid(float DeltaTime, ABoidActor* Boid, const FVector& Pos) const
{
	Boid->Acceleration = Separate(Boid, Pos) + Align(Boid, Pos) + Cohere(Boid, Pos);
	Boid->Acceleration += AvoidFloor(Boid, Pos) + AvoidBoundary(Boid, Pos);
	Boid->Acceleration += FMath::FRandRange(0.f, Boid->VarianceTerm) * FMath::VRand();
	Boid->Velocity += DeltaTime * Boid->Acceleration;
	if (Boid->Velocity.Size() > Boid->MaxSpeed)
	{
		Boid->Velocity = Boid->Velocity.GetSafeNormal() * Boid->MaxSpeed;
	}
	Boid->AddActorWorldOffset(DeltaTime * Boid->Velocity);
}

const FVector UBoidSubsystem::Separate(ABoidActor* Boid, const FVector& Pos) const
{
	const TArray<ABoidActor*> LocalBoids = BoidOctree->GetBoids(Pos, Boid->SeparationThreshold);
	if (LocalBoids.Num() == 1)
	{
		return FVector::ZeroVector;
	}
	FVector Res = FVector::ZeroVector;
	FVector Position;
	for (int i = 0; i < LocalBoids.Num(); i++)
	{
		Position = Boids[i]->GetActorLocation();
		if (LocalBoids[i] == Boid || FVector::Distance(Position, Pos) > Boid->SeparationThreshold)
		{
			continue;
		}
		Res += Pos - Position;
	}
	return Boid->SeparationTerm * Res;
}

const FVector UBoidSubsystem::Align(ABoidActor* Boid, const FVector& Pos) const
{
	const TArray<ABoidActor*> LocalBoids = BoidOctree->GetBoids(Pos, Boid->AlignmentThreshold);
	if (LocalBoids.Num() == 1)
	{
		return FVector::ZeroVector;
	}
	FVector Res = FVector::ZeroVector;
	 for (int i = 0; i < LocalBoids.Num(); i++)
	{
		if (LocalBoids[i] == Boid || FVector::Distance(LocalBoids[i]->GetActorLocation(), Pos) > Boid->AlignmentThreshold)
		{
			continue;
		}
		Res += LocalBoids[i]->Velocity;
	}
	Res /= LocalBoids.Num() - 1.f;
	return Boid->AlignmentTerm * Res;
}

const FVector UBoidSubsystem::Cohere(ABoidActor* Boid, const FVector& Pos) const
{
	const TArray<ABoidActor*> LocalBoids = BoidOctree->GetBoids(Pos, Boid->CohesionThreshold);
	if (LocalBoids.Num() == 1)
	{
		return FVector::ZeroVector;
	}
	FVector Res = FVector::ZeroVector;
	FVector Position;
	for (int i = 0; i < LocalBoids.Num(); i++)
	{
		Position = Boids[i]->GetActorLocation();
		if (LocalBoids[i] == Boid || FVector::Distance(Position, Pos) > Boid->CohesionThreshold)
		{
			continue;
		}
		Res += Position;
	}
	Res /= LocalBoids.Num() - 1.f;
	return Boid->CohesionTerm * Res;
}

const FVector UBoidSubsystem::AvoidFloor(ABoidActor* Boid, const FVector& Pos) const
{
	return Boid->AvoidanceTerm * FMath::Max(Boid->AvoidanceThreshold - Pos.Z + FLOOR_HEIGHT, 0.f) * FVector::UpVector;
}

const FVector UBoidSubsystem::AvoidBoundary(ABoidActor* Boid, const FVector& Pos) const
{
	const float Len = Pos.Length();
	return Boid->AvoidanceTerm * (-Pos / Len) * FMath::Max(Len + Boid->AvoidanceThreshold - Boid->BoundaryRadius, 0.f);
}
