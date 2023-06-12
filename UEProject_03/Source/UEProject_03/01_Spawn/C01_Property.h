#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C01_Property.generated.h"

UCLASS()
class UEPROJECT_03_API AC01_Property : public AActor
{
	GENERATED_BODY()

private:
	// �ؿ��� �Ѵ� ��ģ��. ���������� �ٲ� �� �ְ� �ν��Ͻ������� �ٲ� �� ����
	UPROPERTY(EditAnywhere)
		int32 A = 10;
	// �߰��� ������ �Ұ����ϴ�. ��ġ ���� ���� ���� ����
	UPROPERTY(EditInstanceOnly)
		int32 B;
	// Editor������ �ٲ� �� �ִ�. C���� �ٲٱ� X
	UPROPERTY(EditDefaultsOnly)
		int32 C;
	// ���ǻ���: InstanceOnly�� DefaultOnly�� Actor ���Ͽ����� ����� �� �ִ�.

	// ������ �� ���� �� �뵵 Ȯ�ο����� ����
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
