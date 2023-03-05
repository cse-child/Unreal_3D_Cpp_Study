#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class UEPROJECT_03_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

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

public:
	UFUNCTION(BlueprintCallable)
		void ChangeColor(FLinearColor InColor);

private:
	TArray<class UMaterialInstanceDynamic*> Materials;
};
