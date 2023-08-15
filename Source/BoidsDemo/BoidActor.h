// Lincensed under the WTFPL v2

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidActor.generated.h"

UCLASS()
class BOIDSDEMO_API ABoidActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin=0.f))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float SeparationThreshold;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float SeparationTerm;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float AlignmentThreshold;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float AlignmentTerm;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float CohesionThreshold;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float CohesionTerm;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float AvoidanceThreshold;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float AvoidanceTerm;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float BoundaryRadius;

	UPROPERTY(EditAnywhere, meta=(UIMin=0.f))
	float VarianceTerm;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly)
	FVector Acceleration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAppearance(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void FaceVelocity(const float RollSpringCoefficient);
};
