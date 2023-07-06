#pragma once

#include "CoreMinimal.h"
#include "ICharacter.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Parkour/CParkourComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class UEPROJECT_06_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 1;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCParkourComponent* Parkour;

	UPROPERTY(VisibleDefaultsOnly)
		class UCZoomComponent* Zoom;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleDefaultsOnly)
		class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); } // 

private:
	void OnAvoid();

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void BackStep();

public:
	void End_BackStep() override;

private:
	void OnRightButton();

public:
	virtual void Landed(const FHitResult& Hit) override;

private:
	void SetZooming(float InValue);
};

