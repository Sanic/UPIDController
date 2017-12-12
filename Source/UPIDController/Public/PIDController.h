// Copyright 2017, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "EngineMinimal.h"
#include "PIDController.generated.h"

/**
* PID Controller
* Error: where you are vs where you want to be
* Derivative: how fast you are approaching, dampening
* Integral: alignment error
*/
USTRUCT(BlueprintType)
struct UPIDCONTROLLER_API FPIDController 
{
	GENERATED_USTRUCT_BODY()

public:
	// Proportional gain
	UPROPERTY(EditAnywhere)
	float P;

	// Integral gain
	UPROPERTY(EditAnywhere)
	float I;

	// Derivative gain
	UPROPERTY(EditAnywhere)
	float D;

	// Max output (as absolute value)
	UPROPERTY(EditAnywhere)
	float MaxOutAbs;

	// Default constructor (no initialization)
	FPIDController() { }

	// Constructor with initial value for each component
	FPIDController(float InP, float InI, float InD, float InMaxOutAbs);

	// Update the PID loop
	float Update(const float InError, const float InDeltaTime);

	// Update as a PID controller
	float UpdateAsPID(const float InError, const float InDeltaTime);

	// Update as a P controller
	float UpdateAsP(const float InError);

	// Update as a PD controller
	float UpdateAsPD(const float InError, const float InDeltaTime);

	// Update as a PI controller
	float UpdateAsPI(const float InError, const float InDeltaTime);

	// Reset error values of the PID
	void Reset();
	
private:
	// Previous step error value
	float PrevErr;

	// Integral error
	float IErr;
};

/* FPIDController inline functions
*****************************************************************************/
FORCEINLINE FPIDController::FPIDController(float InP, float InI, float InD, float InMaxOutAbs)
	: P(InP), I(InI), D(InD), MaxOutAbs(InMaxOutAbs)
{ 
	FPIDController::Reset();
}

FORCEINLINE float FPIDController::Update(const float InError, const float InDeltaTime)
{
	if (InDeltaTime == 0.0f || FMath::IsNaN(InError))
	{
		return 0.0f;
	}

	// Calculate proportional output
	const float POut = P * InError;

	// Calculate integral error / output
	IErr += InDeltaTime * InError;
	const float IOut = I * IErr;

	// Calculate the derivative error / output
	const float DErr = (InError - PrevErr) / InDeltaTime;
	const float DOut = D * DErr;

	// Set previous error
	PrevErr = InError;

	// Calculate the output
	const float Out = POut + IOut + DOut;
	
	// Clamp output
	return FMath::Clamp(Out, -MaxOutAbs, MaxOutAbs);
}

FORCEINLINE float FPIDController::UpdateAsPID(const float InError, const float InDeltaTime)
{
	return FPIDController::Update(InError, InDeltaTime);
}

FORCEINLINE float FPIDController::UpdateAsP(const float InError)
{
	if (FMath::IsNaN(InError))
	{
		return 0.0f;
	}

	// Calculate proportional output
	const float Out = P * InError;

	// Clamp output
	return FMath::Clamp(Out, -MaxOutAbs, MaxOutAbs);
}

FORCEINLINE float FPIDController::UpdateAsPD(const float InError, const float InDeltaTime)
{
	if (InDeltaTime == 0.0f || FMath::IsNaN(InError))
	{
		return 0.0f;
	}

	// Calculate proportional output
	const float POut = P * InError;

	// Calculate the derivative error / output
	const float DErr = (InError - PrevErr) / InDeltaTime;
	const float DOut = D * DErr;

	// Set previous error
	PrevErr = InError;

	// Calculate the output
	const float Out = POut + DOut;

	// Clamp output
	return FMath::Clamp(Out, -MaxOutAbs, MaxOutAbs);
}

FORCEINLINE float FPIDController::UpdateAsPI(const float InError, const float InDeltaTime)
{
	if (InDeltaTime == 0.0f || FMath::IsNaN(InError))
	{
		return 0.0f;
	}

	// Calculate proportional output
	const float POut = P * InError;

	// Calculate integral error / output
	IErr += InDeltaTime * InError;
	const float IOut = I * IErr;

	// Calculate the output
	const float Out = POut + IOut;

	// Clamp output
	return FMath::Clamp(Out, -MaxOutAbs, MaxOutAbs);
}

FORCEINLINE void FPIDController::Reset()
{
	PrevErr = 0.f;
	IErr = 0.f;
}

/* DEPRECATED, moved as USTRUCT */
/**
* PID Controller
* Error: where you are vs where you want to be
* Derivative: how fast you are approaching, dampening
* Integral: alignment error
*/
class UPIDCONTROLLER_API PIDController
{
public:
	// Default constructor
	PIDController();

	// Constructor
	PIDController(float ProportionalVal, float IntegralVal, float DerivativeVal,
		float OutMaxVal = 0.f, float OutMinVal = 0.f);

	// Destructor
	~PIDController();

	// Set all PID values
	void SetValues(float ProportionalVal = 0.f, float IntegralVal = 0.f, float DerivativeVal = 0.f,
		float OutMaxVal = 0.f, float OutMinVal = 0.f);

	// Update the PID loop
	float Update(const float Error, const float DeltaTime);

	// Update only P
	float UpdateAsP(const float Error, const float DeltaTime);

	// Update only PD
	float UpdateAsPD(const float Error, const float DeltaTime);

	// Update only P�
	float UpdateAsPI(const float Error, const float DeltaTime);

	// Reset error values of the PID
	void Reset();

private:
	// Proportional gain
	float P;

	// Integral gain
	float I;

	// Derivative gain
	float D;

	// Output maximul clamping value
	float OutMax;

	// Output minimum claming value
	float OutMin;

	// Previous step error value
	float PrevErr;

	// Integral error
	float IErr;
};
