#include "StaticMesh_Detail.h"
#include "CStaticMesh.h"
#include "DesktopPlatformModule.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDesktopPlatform.h"
#include "Engine/StaticMeshSocket.h"
#include "Interfaces/IMainFrameModule.h"
#include "Misc/MessageDialog.h"

TSharedRef<IDetailCustomization> FStaticMesh_Detail::MakeInstance()
{
	return MakeShareable(new FStaticMesh_Detail());
}

void FStaticMesh_Detail::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& mesh = DetailBuilder.EditCategory("Mesh");

	mesh.AddCustomRow(FText::FromString("Color")) // 검색 기준
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Color"))
	]
	.ValueContent()
	.VAlign(VAlign_Center)		// 가운데 정렬
	.MaxDesiredWidth(250)		// 최대 넓이
	[
		SNew(SButton)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// 꽉 채운 상태로 시작
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Paint)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Paint"))
		]
	];

	mesh.AddCustomRow(FText::FromString("SaveMesh")) // 검색 기준
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Mesh"))
		]
	.ValueContent()
		.VAlign(VAlign_Center)		// 가운데 정렬
		.MaxDesiredWidth(250)		// 최대 넓이
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// 꽉 채운 상태로 시작
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Save)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Save"))
		]
		];

	// 클릭한 Object의 List를 반환
	DetailBuilder.GetObjectsBeingCustomized(Objects);
}

FReply FStaticMesh_Detail::OnClicked_Paint()
{
	for(TWeakObjectPtr<UObject> obj : Objects)
	{
		ACStaticMesh* meshActor = Cast<ACStaticMesh>(obj);

		if (!!meshActor)
			meshActor->Paint();
	}

	return FReply::Handled();
}

FReply FStaticMesh_Detail::OnClicked_Save()
{
	// 선택한 Mesh 전부 meshActors에 저장하기
	TArray<ACStaticMesh*> meshActors;
	for(TWeakObjectPtr<UObject> obj : Objects)
	{
		ACStaticMesh* meshActor = Cast<ACStaticMesh>(obj);
		if (meshActor == nullptr) continue;

		meshActors.Add(meshActor);
	}


	// Message Dialog Test
	//{
	//	FText title = FText::FromString("Warning");
	//	FText context = FText::FromString(FString::Printf(L"Selected Num : %d", meshActors.Num()));
	//	FMessageDialog::Debugf(context, &title);
	//}


	// 선택된 Mesh들의 Save 기능 구현
	for(int32 i = 0; i < meshActors.Num(); i++)
	{
		UActorComponent* actorComponent = meshActors[i]->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(actorComponent);

		// mesh Component가 존재하지 않아서 처리할 버튼 명령이 없다면 다음 레이어로 Pass
		if (meshComponent == nullptr) FReply::Unhandled();

		// 실제로 화면에 그려지는 것은 Mesh 컴포넌트 안에있는 Static Mesh
		UStaticMesh* mesh = meshComponent->GetStaticMesh();
		FStaticMeshRenderData* renderData = mesh->RenderData.Get();

		// 그릴 데이터가 없다면 Pass
		if (renderData->LODResources.Num() < 1) return FReply::Unhandled();

		FStaticMeshLODResources& lod = renderData->LODResources[0];
		FPositionVertexBuffer& vb = lod.VertexBuffers.PositionVertexBuffer;
		FStaticMeshVertexBuffer& meshVB = lod.VertexBuffers.StaticMeshVertexBuffer; // Normal, UV 같이 들어가있음
		FColorVertexBuffer& colorVB = lod.VertexBuffers.ColorVertexBuffer;
		FRawStaticIndexBuffer& ib = lod.IndexBuffer;

		// Position 버퍼 위치의 정점 개수 == 버퍼의 원소 개수
		//vb.GetNumVertices();

		uint32 vertexCount = vb.GetNumVertices();
		int32 indexCount = ib.GetNumIndices();

		GLog->Logf(L"%d Mesh, Vertex Count : %d", i+1, vertexCount);
		GLog->Logf(L"%d Mesh, Index Count : %d", i+1, indexCount);
		GLog->Logf(L"%d Mesh, Triangle Count : %d", i+1, indexCount / 3);


		FString path;
		// FPaths::ConvertRelativePathToFull() : 상대경로를 절대경로로 변경
		FPaths::GetPath(path);

		IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		// 핸들: 해당 객체의 식별자이자, 그 객체의 시작주소
		// ex. 윈도우 핸들 : 윈도우의 식별자
		// DWORD (Double WORD)
		void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();

		
	}

	//IDesktopPlatform* platform = FDesktopPlatformModule::Get();
	//
	//TArray<FString> fileNames;
	//platform->SaveFileDialog(handle, "Save Dialog", path, "", "Mesh Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);
	//
	//if (fileNames.Num() < 1) return FReply::Unhandled(); // 선택한 파일이 없다면 Return
	//
	//GLog->Log(fileNames[0]);

	return FReply::Handled();
}
