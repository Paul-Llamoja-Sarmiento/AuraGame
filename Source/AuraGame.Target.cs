

using UnrealBuildTool;
using System.Collections.Generic;

public class AuraGameTarget : TargetRules
{
	public AuraGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "AuraGame" } );
	}
}
