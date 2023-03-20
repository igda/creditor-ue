#include "Creditor.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Font.h"
#include "Misc/Paths.h"

#include <rapidcsv.h>

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
  FString fullCSVPath = FPaths::Combine(FPaths::ProjectContentDir(), CreditsFile);

  UE_LOG(Creditor, Log, TEXT("Populating credits from CSV file \"%s\""), *fullCSVPath);

  if (!FPaths::FileExists(fullCSVPath))
  {
    UE_LOG(Creditor, Error, TEXT("Could not find CreditsFile at \"%s\""), *fullCSVPath);
    return;
  }

  std::string fullCSVPathAsString(TCHAR_TO_UTF8(*fullCSVPath));
  rapidcsv::Document doc(fullCSVPathAsString);

  const size_t numCellRows = doc.GetRowCount();

  auto allRowsWidget = NewObject<UVerticalBox>(CanvasPanel);
  CanvasPanel->AddChild(allRowsWidget);

  for (size_t cellRowIdx = 0u; cellRowIdx < numCellRows; ++cellRowIdx)
  {
    FString format = doc.GetCell<std::string>(0, cellRowIdx).c_str();

    auto sectionColumnFormats = IdsToColumnFormats.Find(format);
    const int32 numColumnFormats = sectionColumnFormats->ColumnFormats.Num();
    if (nullptr == sectionColumnFormats || 0 == numColumnFormats)
    {
      UE_LOG(Creditor, Error, TEXT("Could not find formats for section \"%s\""), *format);
      continue;
    }

    // Create a row
    auto rowWidget = NewObject<UHorizontalBox>(allRowsWidget);
    allRowsWidget->AddChild(rowWidget);

    // Make sure this fills the space available in the canvas.
    auto canvasSlot = Cast<UCanvasPanelSlot>(allRowsWidget->Slot.Get());
    if (nullptr == canvasSlot)
    {
      UE_LOG(Creditor, Error, TEXT("Could not find the canvas slot for cell row %d"), cellRowIdx);
      continue;
    }

    canvasSlot->SetAutoSize(true);

    for (int32 widgetColumn = 0; widgetColumn < numColumnFormats; ++widgetColumn)
    {
      TSubclassOf<UCreditorFormat> subclassOfCreditorFormat = sectionColumnFormats->ColumnFormats[widgetColumn];
      if (nullptr == subclassOfCreditorFormat)
      {
        UE_LOG(Creditor, Error, TEXT("Could not find format at index %d of section \"%s\""), widgetColumn, *format);
        continue;
      }

      // Create a column to put our new entry.
      auto colWidget = NewObject<UVerticalBox>(rowWidget);
      rowWidget->AddChild(colWidget);

      // Make sure this slot fills any space possible.
      auto colBoxSlot = Cast<UHorizontalBoxSlot>(colWidget->Slot.Get());
      check(colBoxSlot);
      colBoxSlot->SetSize(ESlateSizeRule::Fill);

      // Determine what we need to fill this entry with.
      UCreditorFormat* creditorFormat = subclassOfCreditorFormat.GetDefaultObject();
      if (nullptr != creditorFormat)
      {
        if (ECreditorFormatType::Text == creditorFormat->Type)
        {
          // The first column is the format specifier, content is in subsequent columns.
          size_t cellColumnIdx = widgetColumn + 1;
          FString cellText = doc.GetCell<std::string>(cellColumnIdx, cellRowIdx).c_str();

          auto tb = NewObject<UTextBlock>(colWidget, UTextBlock::StaticClass());

          tb->SetText(FText::FromString(cellText));

          FTextBlockStyle& style = creditorFormat->TextBlockStyle;
          tb->SetFont(style.Font);
          tb->SetColorAndOpacity(style.ColorAndOpacity);
          tb->SetShadowOffset(style.ShadowOffset);
          tb->SetShadowColorAndOpacity(style.ShadowColorAndOpacity);
          tb->SetStrikeBrush(style.StrikeBrush);

          colWidget->AddChild(tb);
        }
        else // if (ECreditorFormatType::Image == creditorFormat->Type)
        {
          auto image = NewObject<UImage>(colWidget, UImage::StaticClass());

          image->SetBrush(creditorFormat->Brush);

          colWidget->AddChild(image);
        }
      }
    }
  }

  UE_LOG(Creditor, Log, TEXT("Done populating credits from CSV file \"%s\""), *fullCSVPath);
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