#include "PNInputConfig.h"
#include "PNLogChannels.h"

const UInputAction* UPNInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FPNInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogPN, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}
