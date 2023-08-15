// Lincensed under the WTFPL v2

#pragma once

#include "CoreMinimal.h"
#include "Math/Box.h"

/**
 * https://www.gamedev.net/articles/programming/general-and-gameplay-programming/introduction-to-octrees-r3529/
 * https://docs.unrealengine.com/4.26/en-US/API/Runtime/Core/Math/FBox/ 
 */
class BOIDSDEMO_API FOctree
{
public:

	FOctree(int N, float MinDimension);
	FOctree(TArray<class ABoidActor*> Boids, int N, float MinDimension);
	// assume you'll only explicity call destructor on the root node
	~FOctree();

	TArray<class ABoidActor*> Boids;
	FOctree* Parent;

protected:

	int N;
	float MinDimension;
	FBox Region;
	TArray<FOctree*> Children;

public:

	void Add(class ABoidActor* Boid);

	void Build();

	void Build(FBox& Box);

	void EmptyChildren();

	// assume you'll only call this on the root node
	TArray<class ABoidActor*> GetBoids(FVector Center, float Radius) const;

	bool Contains(FVector& Point, float Radius) const;

protected:

	static FBox GetRegion(TArray<class ABoidActor*> Items);
};
