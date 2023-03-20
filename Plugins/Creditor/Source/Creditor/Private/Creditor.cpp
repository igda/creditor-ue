#include "Creditor.h"

#include "Engine/Font.h"

static const FName CreditorTabName("Creditor");

#define LOCTEXT_NAMESPACE "FCreditorModule"

DEFINE_LOG_CATEGORY(Creditor);

UCreditorFormat::UCreditorFormat()
  : Type(ECreditorFormatType::Text)
{
  static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
  TextBlockStyle.SetFont(FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold")));
  TextBlockStyle.SetColorAndOpacity(FColor::White);
  TextBlockStyle.SetShadowColorAndOpacity(FColor::Black);
}

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{}

UCreditsWidget::~UCreditsWidget()
{}

void UCreditsWidget::Populate(UCanvasPanel* CanvasPanel)
{
  (void)CanvasPanel;
}


void FCreditorModule::StartupModule()
{
  // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCreditorModule::ShutdownModule()
{
  // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
  // we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCreditorModule, Creditor)