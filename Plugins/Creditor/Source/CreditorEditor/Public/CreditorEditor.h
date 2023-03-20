// Copyright International Game Developers Association (IGDA). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "IDetailCustomization.h"
#include "IPropertyTypeCustomization.h"

DECLARE_LOG_CATEGORY_EXTERN(CreditorEditor, Log, All);

class FToolBarBuilder;
class FMenuBuilder;

class CREDITOREDITOR_API UCreditorFormatDetailCustomization : public IDetailCustomization
{
public:
  static TSharedRef<IDetailCustomization> MakeInstance();

  void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};

class CREDITOREDITOR_API UCreditorFormatPropertyTypeCustomization : public IPropertyTypeCustomization
{
public:
  static TSharedRef<IPropertyTypeCustomization> MakeInstance();

  void OnTypeChanged(TSharedPtr<IPropertyHandle> PropertyHandle, TSharedPtr<IPropertyUtilities> Utils);

  void CustomizeHeader(
    TSharedRef<IPropertyHandle> PropertyHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& PropertyTypeCustomizationUtils) override;

  void CustomizeChildren(
    TSharedRef<IPropertyHandle> PropertyHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};

class CREDITOREDITOR_API FCreditorEditorModule : public IModuleInterface
{
public:

  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};
