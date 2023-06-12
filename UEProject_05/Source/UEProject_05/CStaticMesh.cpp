#include "CStaticMesh.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"


ACStaticMesh::ACStaticMesh()
{
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/MatineeCam_SM.MatineeCam_SM'");
	Mesh->SetStaticMesh(mesh);

	CHelpers::GetAsset<UMaterialInstanceConstant>(&Material, "MaterialInstanceConstant'/Game/M_StaticMesh_Inst.M_StaticMesh_Inst'");
	Mesh->SetMaterial(0, Material);

}

void ACStaticMesh::BeginPlay()
{
	Super::BeginPlay();
}

#if WITH_EDITOR
void ACStaticMesh::Paint()
{
	for(const FVectorParameterValue& value :  Material->VectorParameterValues)
	{
		// SetVectorParameterValueEditorOnly 함수의 원형이 WITH_EDITOR로 선언되어있기 때문에
		// 같은 범위인 WITH_EDITOR로 호출해야 Crash 나지 않는다.
		if (value.ParameterInfo.Name.Compare("Color") == 0) // -1, 1 나오면 다른 문자열
			Material->SetVectorParameterValueEditorOnly(value.ParameterInfo, FLinearColor::MakeRandomColor());
	}
}
#endif

