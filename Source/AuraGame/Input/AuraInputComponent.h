
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Data/InputConfiguration.h"
#include "AuraInputComponent.generated.h"


UCLASS()
class AURAGAME_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UInputConfiguration* InputConfiguration, UserClass* UserObject,
	                        PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UInputConfiguration* InputConfiguration,
	UserClass* UserObject, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfiguration);

	for (const FInputActionTagMapping& Mapping : InputConfiguration->InputActionMappings)
	{
		if (!IsValid(Mapping.InputAction) || !Mapping.InputTag.IsValid()) continue;

		if (PressedFunc)
		{
			BindAction(Mapping.InputAction, ETriggerEvent::Started, UserObject, PressedFunc, Mapping.InputTag);
		}

		if (ReleasedFunc)
		{
			BindAction(Mapping.InputAction, ETriggerEvent::Completed, UserObject, ReleasedFunc, Mapping.InputTag);
		}

		if (HeldFunc)
		{
			BindAction(Mapping.InputAction, ETriggerEvent::Triggered, UserObject, HeldFunc, Mapping.InputTag);
		}
	}
}
