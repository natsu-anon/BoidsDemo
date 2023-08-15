// Lincensed under the WTFPL v2


#include "Octree.h"
#include "BoidActor.h"

FOctree::FOctree(int N, float MinDimension)
{
	this->N = N;
	this->MinDimension = MinDimension;
}

FOctree::FOctree(TArray<ABoidActor*> Boids, int N, float MinDimension)
{
	this->Region = GetRegion(Boids);
	this->Boids = Boids;
	this->N = N;
	this->MinDimension = MinDimension;
}

FOctree::~FOctree()
{
	Boids.Empty();
	for (int i = Children.Num() - 1; i >= 0; i--)
	{
		delete Children[i];
		Children.RemoveAt(i);
	}
}

void FOctree::Add(ABoidActor* Boid)
{
	Boids.Add(Boid);
}

void FOctree::Build()
{
	FBox Temp = GetRegion(Boids);
	Build(Temp);
}

void FOctree::Build(FBox& Box)
{
	this->Region = Box;
	if (Boids.Num() > N)
	{
		FVector Center, Extents;
		Region.GetCenterAndExtents(Center, Extents);
		if (Extents.GetMin() < MinDimension)
		{
			return;
		}
		Extents *= 0.5f;
		TArray<FBox> Subregions = TArray<FBox>();
		Subregions.Add(FBox::BuildAABB(Center - Extents, Extents));
		Subregions.Add(FBox::BuildAABB(Center + FVector(1.f, -1.f, -1.f) * Extents, Extents));
		Subregions.Add(FBox::BuildAABB(Center + FVector(-1.f, 1.f, -1.f) * Extents, Extents));
		Subregions.Add(FBox::BuildAABB(Center + FVector(-1.f, -1.f, 1.f) * Extents, Extents));
		Subregions.Add(FBox::BuildAABB(Center + FVector(-1.f, 1.f, 1.f) * Extents, Extents));
		Subregions.Add(FBox::BuildAABB(Center + FVector(1.f, -1.f, 1.f) * Extents, Extents));
		Subregions.Add(FBox::BuildAABB(Center + FVector(1.f, 1.f, -1.f) * Extents, Extents));
		Subregions.Add(FBox::BuildAABB(Center + Extents, Extents));
		for (int i = 0; i < 8; i++)
		{
			Children.Add(new FOctree(N, MinDimension));
		}
		for (int i = 0; i < Boids.Num(); i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (Subregions[j].IsInsideOrOn(Boids[i]->GetActorLocation()))
				{
					Children[j]->Add(Boids[i]);
					break;
				}
			}
		}
		for (int i = 0; i < 8; i++)
		{
			Children[i]->Build(Subregions[i]);
		}
	}
}

void FOctree::EmptyChildren()
{
	for (int i = Children.Num() - 1; i >= 0; i--)
	{
		delete Children[i];
		Children.RemoveAt(i);
	}
}

TArray<ABoidActor*> FOctree::GetBoids(FVector Center, float Radius) const
{
	for (int i = 0; i < Children.Num(); i++)
	{
		if (Children[i]->Contains(Center, Radius))
		{
			return Children[i]->GetBoids(Center, Radius);
		}
	}
	return Boids;
}

bool FOctree::Contains(FVector& Point, float Radius) const
{
	FVector Center, Extents;
	Region.GetCenterAndExtents(Center, Extents);
	return FMath::Abs(Point.X - Center.X) + Radius < Extents.X && FMath::Abs(Point.Y - Center.Y) + Radius < Extents.Y && FMath::Abs(Point.Z - Center.Z) + Radius < Extents.Z; // THINK ABOUT IT!
}

FBox FOctree::GetRegion(TArray<ABoidActor*> Boids)
{
	if (Boids.Num() > 0)
	{
		FVector Minimum = Boids[0]->GetActorLocation();
		FVector Maximum = Boids[0]->GetActorLocation();
		FVector Pos;
		for (int i = 1; i < Boids.Num(); i++)
		{
			Pos = Boids[i]->GetActorLocation();
			if (Pos.X < Minimum.X)
			{
				Minimum.X = Pos.X;
			}
			if (Pos.Y < Minimum.Y)
			{
				Minimum.Y = Pos.Y;
			}
			if (Pos.Z < Minimum.Z)
			{
				Minimum.Z = Pos.Z;
			}
			if (Pos.X > Maximum.X)
			{
				Maximum.X = Pos.X;
			}
			if (Pos.Y > Maximum.Y)
			{
				Maximum.Y = Pos.Y;
			}
			if (Pos.Z > Maximum.Z)
			{
				Maximum.Z = Pos.Z;
			}
		}
		FVector Center = 0.5f * (Minimum + Maximum);
		return FBox::BuildAABB(Center, Maximum - Center);
	}
	else
	{
		return FBox::BuildAABB(FVector::ZeroVector, FVector::ZeroVector);
	}
}
