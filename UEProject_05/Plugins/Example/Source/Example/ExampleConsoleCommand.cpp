#include "ExampleConsoleCommand.h"
#include "GameFramework/Character.h"
#include "HAL/IConsoleManager.h" // HAL(Hardware Abstraction Layer): 하드웨어의 추상화, 하드웨어에 상관없이 작동함
#include "Interfaces/IMainFrameModule.h"

FExampleConsoleCommand::FExampleConsoleCommand()
{
	// VA_ARGS : 가변 함수
	FConsoleCommandDelegate command = FConsoleCommandDelegate::CreateRaw(this, &FExampleConsoleCommand::OpenWindow, FString("None Arguments"));
	OpenWindowCommand = IConsoleManager::Get().RegisterConsoleCommand(L"TestCommand", L"Display Window(none args)", command);

	FConsoleCommandWithArgsDelegate command2;
	command2.BindLambda([&](const TArray<FString>& InArgs)
	{
		FString str;
		str.Append("With Arguments : ");
		str.Append(FString::FromInt(InArgs.Num()));
		str.Append("||");
		str.Append("GameMode : ");
		str.Append(FApp::IsGame() ? "True" : "False");
		str.Append("||");
	
		for(FString args : InArgs)
		{
			str.Append(args);
			str.Append("||");
		}
	
		OpenWindow(str);
	});
	
	OpenWindowArgsCommand = IConsoleManager::Get().RegisterConsoleCommand(L"TestCommandArgs", L"Display Window(with args)", command2);

	FConsoleCommandWithWorldAndArgsDelegate command3;
	command3.BindRaw(this, &FExampleConsoleCommand::MoveToPlayer);

	OpenWindowWorldArgsCommand = IConsoleManager::Get().RegisterConsoleCommand(L"MoveToPlayer", L"Move To Player(FVector Position)", command3);
}

FExampleConsoleCommand::~FExampleConsoleCommand()
{
	if (!!OpenWindowCommand)
		IConsoleManager::Get().UnregisterConsoleObject(OpenWindowCommand);

	if (!!OpenWindowArgsCommand)
		IConsoleManager::Get().UnregisterConsoleObject(OpenWindowArgsCommand);

	if (!!OpenWindowWorldArgsCommand)
		IConsoleManager::Get().UnregisterConsoleObject(OpenWindowWorldArgsCommand);
}

void FExampleConsoleCommand::OpenWindow(FString InString)
{
	//bool b = FApp::IsGame(); // 현재 게임모드인지
	//GLog->Log(ELogVerbosity::Warning, b ? "Game" : "None Game");

	TSharedRef<SVerticalBox> box = SNew(SVerticalBox);
	
	TArray<FString> strs;
	InString.ParseIntoArray(strs, L"||");
	
	for(FString str : strs)
	{
		box.Get().AddSlot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(str))
			];
	}
	
	TSharedRef<SWindow> window = SNew(SWindow)
	.Title(FText::FromString("Console Command Window"))		// Window Title
	.ClientSize(FVector2D(640, 480))						// Window Size
	.SupportsMaximize(false)								
	.SupportsMinimize(false)
	.Content()
	[
		box
	];
	
	IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	
	if (mainFrame.GetParentWindow().IsValid())
	{
		// ToSharedRef: SharedPtr 을 Ref로 변경
		FSlateApplication::Get().AddWindowAsNativeChild(window, mainFrame.GetParentWindow().ToSharedRef());
	
		return;
	}
	
	FSlateApplication::Get().AddWindow(window);
}

void FExampleConsoleCommand::MoveToPlayer(const TArray<FString>& InArgs, UWorld* InWorld)
{
	if(InArgs.Num() != 3)
	{
		GLog->Log("float(X) float(Y) float(Z) must be input correct format");

		return;
	}

	FVector position;
	position.X = FCString::Atof(*InArgs[0]);
	position.Y = FCString::Atof(*InArgs[1]);
	position.Z = FCString::Atof(*InArgs[2]);

	// Game Play 중이라면 Character의 Location 변경
	if(FApp::IsGame())
	{
		APlayerController* controller =  InWorld->GetFirstPlayerController<APlayerController>();
		ACharacter* character = controller->GetPawn<ACharacter>();

		character->SetActorLocation(position);

		return;
	}

	// Game Play 중이 아니라면, Player Start 객체의 Location 변경
	for(AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		if (!!actor && actor->GetName().Contains("PlayerStart"))
			actor->SetActorLocation(position);
	}
}
