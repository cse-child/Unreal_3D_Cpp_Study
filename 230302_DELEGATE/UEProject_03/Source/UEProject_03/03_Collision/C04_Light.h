#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C04_Light.generated.h"

UCLASS()
class UEPROJECT_03_API AC04_Light : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere)
		class UPointLightComponent* PointLight2;

	UPROPERTY(VisibleAnywhere)
		class UTextRenderComponent* Text;
	
public:	
	AC04_Light();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnLight();

	UFUNCTION()
		void OffLight();

	UFUNCTION()
		FString OnRandomColor(FLinearColor InColor);
};
