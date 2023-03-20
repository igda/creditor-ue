// Copyright International Game Developers Association (IGDA). All Rights Reserved.

#include "CreditorEditor.h"

#include "Creditor.h"
#include "DetailLayoutBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"

static const FName CreditorEditorTabName("CreditorEditor");

#define LOCTEXT_NAMESPACE "FCreditorEditorModule"

DEFINE_LOG_CATEGORY(CreditorEditor);

TSharedRef<IDetailCustomization> UCreditorFormatDetailCustomization::MakeInstance()
{
  return MakeShareable(new UCreditorFormatDetailCustomization());
}

void UCreditorFormatDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
  // Acquire and check all the handles for our various properites.
  auto CreditorEditorFormatTypePropertyHandle = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCreditorFormat, Type));
  if (!CreditorEditorFormatTypePropertyHandle->IsValidHandle())
  {
    UE_LOG(CreditorEditor, Error, TEXT("UCreditorFormatDetailCustomization::CustomizeDetails(): Could not find handle for UCreditorFormat::Type"));
    return;
  }

  auto TextBlockStylePropertyHandle = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCreditorFormat, TextBlockStyle));
  if (!TextBlockStylePropertyHandle->IsValidHandle())
  {
    UE_LOG(CreditorEditor, Error, TEXT("UCreditorFormatDetailCustomization::CustomizeDetails(): Could not find handle for UCreditorFormat::TextBlockStyle"));
    return;
  }

  auto BrushPropertyHandle = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UCreditorFormat, Brush));
  if (!BrushPropertyHandle->IsValidHandle())
  {
    UE_LOG(CreditorEditor, Error, TEXT("UCreditorFormatDetailCustomization::CustomizeDetails(): Could not find handle for UCreditorFormat::Brush"));
    return;
  }

  // Listen for changes to the format type.
  CreditorEditorFormatTypePropertyHandle->SetOnPropertyValueChanged(
    FSimpleDelegate::CreateLambda([&DetailLayout]()
      {
        DetailLayout.ForceRefreshDetails();
      }
    )
  );

  // Mark the optional properties as customized so they are hidden by default.
  DetailLayout.HideProperty(TextBlockStylePropertyHandle);
  DetailLayout.HideProperty(BrushPropertyHandle);

  // Always put tye format type at the top.
  DetailLayout.AddPropertyToCategory(CreditorEditorFormatTypePropertyHandle);

  // Check what the format type is set to.
  uint8 formatRawValue = 0u;
  auto result = CreditorEditorFormatTypePropertyHandle->GetValue(formatRawValue);
  ECreditorFormatType CreditorEditorFormatType = static_cast<ECreditorFormatType>(formatRawValue);

  if (ECreditorFormatType::Text == CreditorEditorFormatType)
  {
    // Only display image properties if this has text format data.
    DetailLayout.AddPropertyToCategory(TextBlockStylePropertyHandle);
  }
  else // if (ECreditorFormatType::Image == CreditorEditorFormatType)
  {
    // Only display image properties if this has image format data.
    DetailLayout.AddPropertyToCategory(BrushPropertyHandle);
  }
}

TSharedRef<IPropertyTypeCustomization> UCreditorFormatPropertyTypeCustomization::MakeInstance()
{
  return MakeShareable(new UCreditorFormatPropertyTypeCustomization());
}

void UCreditorFormatPropertyTypeCustomization::OnTypeChanged(TSharedPtr<IPropertyHandle> PropertyHandle, TSharedPtr<IPropertyUtilities> Utils)
{
  // If the format type has changed, refresh the detail panel contents.
  if (Utils)
  {
    Utils->ForceRefresh();
  }
}

void UCreditorFormatPropertyTypeCustomization::CustomizeHeader(
  TSharedRef<IPropertyHandle> PropertyHandle,
  FDetailWidgetRow& HeaderRow,
  IPropertyTypeCustomizationUtils& PropertyTypeCustomizationUtils)
{
  auto CreditorEditorFormatTypePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UCreditorFormat, Type));
  if (!CreditorEditorFormatTypePropertyHandle.IsValid())
  {
    UE_LOG(CreditorEditor, Error, TEXT("UCreditorFormatPropertyTypeCustomization::Header(): Could not find handle for UCreditorFormat::Type"));
    return;
  }

  OnTypeChanged(CreditorEditorFormatTypePropertyHandle, nullptr);

  // Listen for changes to the format type.
  CreditorEditorFormatTypePropertyHandle->SetOnPropertyValueChanged(
    FSimpleDelegate::CreateSP(
      this,
      &UCreditorFormatPropertyTypeCustomization::OnTypeChanged,
      CreditorEditorFormatTypePropertyHandle,
      PropertyTypeCustomizationUtils.GetPropertyUtilities())
  );
}

void UCreditorFormatPropertyTypeCustomization::CustomizeChildren(
  TSharedRef<IPropertyHandle> PropertyHandle,
  IDetailChildrenBuilder& ChildBuilder,
  IPropertyTypeCustomizationUtils& CustomizationUtils)
{
  auto CreditorEditorFormatTypePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UCreditorFormat, Type));
  if (!CreditorEditorFormatTypePropertyHandle.IsValid())
  {
    UE_LOG(CreditorEditor, Error, TEXT("UCreditorFormatPropertyTypeCustomization::CustomizeChildren(): Could not find handle for UCreditorFormat::Type"));
    return;
  }

  ChildBuilder.AddProperty(CreditorEditorFormatTypePropertyHandle.ToSharedRef());

  // Check what the format type is set to.
  uint8 formatRawValue = 0u;
  auto result = CreditorEditorFormatTypePropertyHandle->GetValue(formatRawValue);
  ECreditorFormatType CreditorEditorFormatType = static_cast<ECreditorFormatType>(formatRawValue);

  if (ECreditorFormatType::Text == CreditorEditorFormatType)
  {
    // Only display image properties if this has text format data.
    auto TextBlockStylePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UCreditorFormat, TextBlockStyle));
    if (!TextBlockStylePropertyHandle.IsValid())
    {
      UE_LOG(CreditorEditor, Error, TEXT("UCreditorFormatPropertyTypeCustomization::CustomizeChildren(): Could not find handle for UCreditorFormat::TextBlockStyle"));
      return;
    }

    ChildBuilder.AddProperty(TextBlockStylePropertyHandle.ToSharedRef());
  }
  else // if (ECreditorFormatType::Image == CreditorEditorFormatType)
  {
    // Only display image properties if this has image format data.
    auto BrushPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UCreditorFormat, Brush));
    if (!BrushPropertyHandle.IsValid())
    {
      UE_LOG(CreditorEditor, Error, TEXT("UCreditorFormatPropertyTypeCustomization::CustomizeChildren(): Could not find handle for UCreditorFormat::Brush"));
      return;
    }

    ChildBuilder.AddProperty(BrushPropertyHandle.ToSharedRef());
  }
}

void FCreditorEditorModule::StartupModule()
{
  // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
  FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
  FName creditorFormatClassName = UCreditorFormat::StaticClass()->GetFName();

  PropertyModule.RegisterCustomClassLayout(
    creditorFormatClassName,
    FOnGetDetailCustomizationInstance::CreateStatic(&UCreditorFormatDetailCustomization::MakeInstance)
  );

  PropertyModule.RegisterCustomPropertyTypeLayout(
    creditorFormatClassName,
    FOnGetPropertyTypeCustomizationInstance::CreateStatic(&UCreditorFormatPropertyTypeCustomization::MakeInstance)
  );

  PropertyModule.NotifyCustomizationModuleChanged();
}

void FCreditorEditorModule::ShutdownModule()
{
  // FModuleManager::LoadModuleChecked<FDetailCustomizationsModule>("DetailCustomizations");

  // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
  // we call this function before unloading the module.

  if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
  {
    FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FName creditorFormatClassName = UCreditorFormat::StaticClass()->GetFName();

    PropertyModule.UnregisterCustomClassLayout(creditorFormatClassName);
    PropertyModule.UnregisterCustomPropertyTypeLayout(creditorFormatClassName);
    PropertyModule.NotifyCustomizationModuleChanged();
  }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCreditorEditorModule, CreditorEditor)