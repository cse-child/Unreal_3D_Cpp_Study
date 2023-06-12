#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C01_Property.generated.h"

UCLASS()
class UEPROJECT_03_API AC01_Property : public AActor
{
	GENERATED_BODY()

private:
	// 밑에꺼 둘다 합친거. 블프에서도 바꿀 수 있고 인스턴스에서도 바꿀 수 있음
	UPROPERTY(EditAnywhere)
		int32 A = 10;
	// 중간에 편집이 불가능하다. 배치 됐을 때만 편집 가능
	UPROPERTY(EditInstanceOnly)
		int32 B;
	// Editor에서만 바꿀 수 있다. C에서 바꾸기 X
	UPROPERTY(EditDefaultsOnly)
		int32 C;
	// 주의사항: InstanceOnly와 DefaultOnly는 Actor 이하에서만 사용할 수 있다.

	// 수정할 수 없는 값 용도 확인용으로 사용됨
	UPROPERTY(VisibleAnywhere, Category = "Property")
		float D = 100;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
		int Variable = 10;

public:	
	AC01_Property();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
