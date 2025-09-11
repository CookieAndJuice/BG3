
#include "Manager/BG3DiceManager.h"

UBG3DiceManager::UBG3DiceManager()
{
	
}

int32 UBG3DiceManager::RollDice(int32 diceNum, int32 maxDice)
{
	int sum = 0;
	for (int i=1; i<=diceNum; i++)
	{
		sum += FMath::RandRange(1, maxDice);
	}
	
	return sum;
}
