// Copyright Vaesty


#include "Character/AuraEnemy.h"
#include "DrawDebugHelpers.h"


void AAuraEnemy::HighlightActor()
{
	bHighlighted = true;
}

void AAuraEnemy::UnhighlightActor()
{
	bHighlighted = false;
}
