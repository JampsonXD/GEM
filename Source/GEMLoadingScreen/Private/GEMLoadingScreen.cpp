#include "GEMLoadingScreen.h"
#include "MoviePlayer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Images/SThrobber.h"

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!
struct FGEMLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FGEMLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if (UObject* CachedResourceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};

class SGEMLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGEMLoadingScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		// Load version of the logo with text baked in, path is hardcoded because this loads very early in startup
		static const FName LoadingScreenName(TEXT("/Game/UI/T_LoadingScreenBackground.T_LoadingScreenBackground"));

		LoadingScreenBrush = MakeShareable(new FGEMLoadingScreenBrush(LoadingScreenName, FVector2D(1920, 1080)));
		
		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.0f);

		ChildSlot
			[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)	
				.Image(LoadingScreenBrush.Get())
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.0f))
				[
					SNew(SThrobber)
					.Visibility(this, &SGEMLoadingScreen::GetLoadIndicatorVisibility)
				]
			]
		];
	}

private:
	/** Rather to show the ... indicator */
	EVisibility GetLoadIndicatorVisibility() const
	{
		bool Vis =  GetMoviePlayer()->IsLoadingFinished();
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}
	
	/** Loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};


class FGEMLoadingScreenModule : public IGEMLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		// Force load the object
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/T_LoadingScreenBackground.T_LoadingScreenBackground"));
		
		if(IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes =!bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(SGEMLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		
	}

	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->Shutdown();
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		// Minimum Duration our Startup loading screen will take
		LoadingScreen.MinimumLoadingScreenDisplayTime = 5.f;
		LoadingScreen.WidgetLoadingScreen = SNew(SGEMLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
};

IMPLEMENT_GAME_MODULE(FGEMLoadingScreenModule, GEMLoadingScreen);
