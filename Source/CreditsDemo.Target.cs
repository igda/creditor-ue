// Copyright International Game Developers Association (IGDA). All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CreditsDemoTarget : TargetRules
{
	public CreditsDemoTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("CreditsDemo");
	}
}
