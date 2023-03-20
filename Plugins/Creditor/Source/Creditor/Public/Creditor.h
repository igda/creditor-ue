#pragma once

#include "CoreMinimal.h"

#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

#include "Creditor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Creditor, Log, All);

UENUM(BlueprintType)
enum class ECreditorFormatType : uint8
{
  Text  UMETA(DisplayName = "Text"),
  Image UMETA(DisplayName = "Image"),
};

UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class CREDITOR_API UCreditorFormat : public UObject
{
  GENERATED_BODY()

public:

  UCreditorFormat();

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  ECreditorFormatType Type;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FTextBlockStyle TextBlockStyle;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FSlateBrush Brush;
};

USTRUCT(BlueprintType)
struct CREDITOR_API FCreditorColumnFormats
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TArray<TSubclassOf<UCreditorFormat>> ColumnFormats;
};

UCLASS(Blueprintable, BlueprintType)
class CREDITOR_API UCreditsWidget : public UUserWidget
{
  GENERATED_BODY()

public:

  UCreditsWidget(const FObjectInitializer& ObjectInitializer);
  virtual ~UCreditsWidget();

  UFUNCTION(BlueprintCallable)
  virtual void Populate(UCanvasPanel* CanvasPanel);

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FString CreditsFile;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TMap<FString, FCreditorColumnFormats> IdsToColumnFormats;
};

class CREDITOR_API FCreditorModule : public IModuleInterface
{
public:

  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};
