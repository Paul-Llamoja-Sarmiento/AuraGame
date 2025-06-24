

using UnrealBuildTool;
using System.Collections.Generic;

public class AuraGameEditorTarget : TargetRules
{
	public AuraGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "AuraGame" } );
	}
}
