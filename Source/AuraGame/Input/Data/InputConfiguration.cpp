
#include "InputConfiguration.h"

// #include "InputAction.h"

// const UInputAction* UInputConfiguration::FindInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
// {
// 	for (const auto& InputActionMapping : InputActionMappings)
// 	{
// 		if (IsValid(InputActionMapping.InputAction) && InputActionMapping.InputTag == InputTag)
// 		{
// 			return InputActionMapping.InputAction;
// 		}
// 	}
//
// 	if (bLogNotFound)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Input action not found for tag: %s"), *InputTag.ToString());
// 	}
//
// 	return nullptr;
// }

#if WITH_EDITOR
#include "Misc/DataValidation.h"


EDataValidationResult FInputActionTagMapping::IsDataValid(FDataValidationContext& Context, const int Index) const
{
	EDataValidationResult Result = EDataValidationResult::Valid;
	if (InputAction == nullptr)
	{
		Result = EDataValidationResult::Invalid;
		const FText ErrorMessage = FText::FromString(FString::Printf(
			TEXT(
				"\n\n An InputAction at index [%i] is none but it mustn't be none.\n Please set a valid class or delete the index entry in the InputActionMappings"),
			Index));
		Context.AddError(ErrorMessage);
	}

	if (!InputTag.IsValid())
	{
		Result = EDataValidationResult::Invalid;
		const FText ErrorMessage = FText::FromString(FString::Printf(
			TEXT(
				"\n\n An InputTag at index [%i] is invalid but it must be valid.\n Please set a valid tag or delete the index entry in the InputActionMappings"),
			Index));
		Context.AddError(ErrorMessage);
	}

	return Result;
}

EDataValidationResult UInputConfiguration::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
	unsigned int Index = 0;
	for (const auto& InputActionMapping : InputActionMappings)
	{
		Result = CombineDataValidationResults(Result, InputActionMapping.IsDataValid(Context, Index));
		Index++;
	}
	
	return Result;
}
#endif

