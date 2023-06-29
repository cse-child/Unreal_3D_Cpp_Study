#include "Components/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#define LOG_UCFeetComponent

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance, rightDistance;
	FRotator leftRotation, rightRotation;

	Trace(LeftSocket, leftDistance, leftRotation);
	Trace(RightSocket, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = offset;

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, ((rightDistance - offset)*-1), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);

#ifdef LOG_UCFeetComponent
	CLog::Print(Data.LeftDistance, 11);
	CLog::Print(Data.RightDistance, 12);
	CLog::Print(Data.PelvisDistance, 13);
	CLog::Print(Data.LeftRotation, 14);
	CLog::Print(Data.RightRotation, 15);
#endif
}

void UCFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	FVector socket = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	float z = OwnerCharacter->GetActorLocation().Z;
	FVector start = FVector(socket.X, socket.Y, z);

	z = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector end = FVector(socket.X, socket.Y, z);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, 
		true, ignores, DrawDebug, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	OutDistance = 0;
	OutRotation = FRotator::ZeroRotator;

	CheckFalse(hitResult.bBlockingHit);

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = length + OffsetDistance - TraceDistance;

	// 밑변과 높이를 알고있기 때문에 atan을 이용하여 회전되야 할 Roll 각도를 구할 수 있다.
	// RotationFromX는 기준값이 다르기 때문에 사용할 수 없다.
	// Roll 회전은 Z축에서 Y축으로 회전되므로, atan이라면 Y축에서 Z축으로 회전되는 것
	// Pitch 회전은 Z축에서 X축으로 회전되므로, atan이라면 X축에서 Z축으로 회전되는 것

	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	float pitch = -1 * UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z);

	OutRotation = FRotator(pitch, 0, roll);
}

