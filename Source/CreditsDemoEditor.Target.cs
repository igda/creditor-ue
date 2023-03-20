// Copyright International Game Developers Association (IGDA). All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CreditsDemoEditorTarget : TargetRules
{
	public CreditsDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("CreditsDemo");
	}
}
