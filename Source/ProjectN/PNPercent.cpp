// Copyright Epic Games, Inc. All Rights Reserved.

#include "PNPercent.h"

#include "PNLogChannels.h"

FPNPercent FPNPercent::FromFraction(int32 Numerator, int32 Denominator)
{
	if (Denominator == 0)
	{
		UE_LOG(LogPN, Error, TEXT("Denominator is 0!"));
		return FPNPercent(0.0f);
	}

	FPNPercent Result;
	Result.RawValue = GetClampedPercent(Numerator * static_cast<int32>(EPercentType::E1Percent) / Denominator * 100);
	
	return Result;
}
