#include "Weapons/AddOns/CCameraModifier.h"
#include "Global.h"

void UCCameraModifier::ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator ViewRotation, float FOV,
	FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV)
{
	Super::ModifyCamera(DeltaTime, ViewLocation, ViewRotation, FOV, NewViewLocation, NewViewRotation, NewFOV);

	// 처음 시작하는 Transform 값
	NewViewLocation = Location;
	NewViewRotation = Rotation;
}
