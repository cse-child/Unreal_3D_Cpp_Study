#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class UEPROJECT_04_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-40, +45);

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Backpack;

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* Arms;

private:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;

public:
	FORCEINLINE class UStaticMeshComponent* GetBackpack() { return Backpack; }
	FORCEINLINE class USkeletalMeshComponent* GetArms() { return Arms; }

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

	void OnRun();
	void OffRun();
};
