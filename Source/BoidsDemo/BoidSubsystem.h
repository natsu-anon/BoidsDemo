// Lincensed under the WTFPL v2

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BoidSubsystem.generated.h"

/**
 * 
 */
UCLASS()

class BOIDSDEMO_API UBoidSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	TArray<class ABoidActor*> Boids;

protected:

	bool bTickable;
	class FOctree* BoidOctree;

public:
	virtual TStatId GetStatId() const override;

	virtual void Tick(float DeltaTime) override;

	void AddBoid(class ABoidActor* Boid);

protected:

	void TickBoid(float DeltaTime, class ABoidActor* Boid, const FVector& Pos) const;

	const FVector Separate(class ABoidActor* Boid, const FVector& Pos) const;

	const FVector Align(class ABoidActor* Boid, const FVector& Pos) const;

	const FVector Cohere(class ABoidActor* Boid, const FVector& Pos) const;

	const FVector AvoidFloor(class ABoidActor* Boid, const FVector& Pos) const;

	const FVector AvoidBoundary(class ABoidActor* Boid, const FVector& Pos) const;
};
