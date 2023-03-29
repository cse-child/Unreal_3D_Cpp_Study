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

	mesh.AddCustomRow(FText::FromString("Color")) // �˻� ����
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Color"))
	]
	.ValueContent()
	.VAlign(VAlign_Center)		// ��� ����
	.MaxDesiredWidth(250)		// �ִ� ����
	[
		SNew(SButton)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// �� ä�� ���·� ����
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Paint)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Paint"))
		]
	];

	mesh.AddCustomRow(FText::FromString("SaveMesh")) // �˻� ����
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Mesh"))
		]
	.ValueContent()
		.VAlign(VAlign_Center)		// ��� ����
		.MaxDesiredWidth(250)		// �ִ� ����
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// �� ä�� ���·� ����
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Save)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Save"))
		]
		];

	// Ŭ���� Object�� List�� ��ȯ
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
	// ������ Mesh ���� meshActors�� �����ϱ�
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


	// ���õ� Mesh���� Save ��� ����
	for(int32 i = 0; i < meshActors.Num(); i++)
	{
		UActorComponent* actorComponent = meshActors[i]->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(actorComponent);

		// mesh Component�� �������� �ʾƼ� ó���� ��ư ����� ���ٸ� ���� ���̾�� Pass
		if (meshComponent == nullptr) FReply::Unhandled();

		// ������ ȭ�鿡 �׷����� ���� Mesh ������Ʈ �ȿ��ִ� Static Mesh
		UStaticMesh* mesh = meshComponent->GetStaticMesh();
		FStaticMeshRenderData* renderData = mesh->RenderData.Get();

		// �׸� �����Ͱ� ���ٸ� Pass
		if (renderData->LODResources.Num() < 1) return FReply::Unhandled();

		FStaticMeshLODResources& lod = renderData->LODResources[0];
		FPositionVertexBuffer& vb = lod.VertexBuffers.PositionVertexBuffer;
		FStaticMeshVertexBuffer& meshVB = lod.VertexBuffers.StaticMeshVertexBuffer; // Normal, UV ���� ������
		FColorVertexBuffer& colorVB = lod.VertexBuffers.ColorVertexBuffer;
		FRawStaticIndexBuffer& ib = lod.IndexBuffer;

		// Position ���� ��ġ�� ���� ���� == ������ ���� ����
		//vb.GetNumVertices();

		uint32 vertexCount = vb.GetNumVertices();
		int32 indexCount = ib.GetNumIndices();

		GLog->Logf(L"%d Mesh, Vertex Count : %d", i+1, vertexCount);
		GLog->Logf(L"%d Mesh, Index Count : %d", i+1, indexCount);
		GLog->Logf(L"%d Mesh, Triangle Count : %d", i+1, indexCount / 3);


		FString path;
		// FPaths::ConvertRelativePathToFull() : ����θ� �����η� ����
		FPaths::GetPath(path);

		IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		// �ڵ�: �ش� ��ü�� �ĺ�������, �� ��ü�� �����ּ�
		// ex. ������ �ڵ� : �������� �ĺ���
		// DWORD (Double WORD)
		void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();

		
	}

	//IDesktopPlatform* platform = FDesktopPlatformModule::Get();
	//
	//TArray<FString> fileNames;
	//platform->SaveFileDialog(handle, "Save Dialog", path, "", "Mesh Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);
	//
	//if (fileNames.Num() < 1) return FReply::Unhandled(); // ������ ������ ���ٸ� Return
	//
	//GLog->Log(fileNames[0]);

	return FReply::Handled();
}
