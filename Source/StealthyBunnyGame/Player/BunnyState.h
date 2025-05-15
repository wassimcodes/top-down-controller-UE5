#pragma once

#include "CoreMinimal.h"

class ABunny;

class BunnyState
{
public:
	virtual ~BunnyState() = default;
	virtual void Enter(ABunny* bunny);
	virtual void Exit(ABunny* bunny);
	virtual void OnMove(ABunny* bunny, const FVector2D& input);

};