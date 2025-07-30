
#include "CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultsInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassDefaultsInfo.FindChecked(CharacterClass);
}
