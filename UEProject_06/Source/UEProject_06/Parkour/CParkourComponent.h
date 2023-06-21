#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CParkourComponent.generated.h"

UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center = 0, Ceil, Floor, Left, Right, Land, Max,
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Climb = 0, Fall, Slide, Short, Normal, Wall, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEPROJECT_06_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 600;

	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float AvailableFrontAngle = 15;

public:	
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void LineTrace(EParkourArrowType InType);

	void CheckTrace_Center();

private:
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

	FHitResult HitResults[(int32)EParkourArrowType::Max];

	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float ToFrontYaw;
};