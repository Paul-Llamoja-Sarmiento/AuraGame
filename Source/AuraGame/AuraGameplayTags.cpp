
#include "AuraGameplayTags.h"


// Vital Attributes
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Vital_Health, "Attributes.Vital.Health", "Amount of damage a character can take before death.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Vital_Mana, "Attributes.Vital.Mana", "A resource used to cast spells.");

// Primary Attributes
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Strength, "Attributes.Primary.Strength", "Increases physical damage.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", "Increases magical damage.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Resilience, "Attributes.Primary.Resilience", "Increases armor and armor penetration.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Vitality, "Attributes.Primary.Vitality", "Increases health");

// Secondary Attributes
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces damage taken. Improves Block Chance. It depends on our Resilience attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", "Increases damage dealt by ignoring a percentage of the target's armor. Increases Critical Hit Chance. It depends on our Resilience attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance", "Chance to cut incoming damage by half. It depends on our Armor attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "Chance to double damage plus critical hit bonus. It depends on our Armor Penetration attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Bonus damage added when a critical hit is scored. It depends on our Armor Penetration attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance", "Reduces critical hit chance of attacking enemies. It depends on our Armor attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "Amount of health restored every 1 second. It depends on our Vigor attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration", "Amount of mana restored every 1 second. It depends on our Intelligence attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum amount of health obtainable. It depends on our Vigor attribute.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana", "Maximum amount of mana obtainable. It depends on our Intelligence attribute.");


// Game Effects Tags
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Message_HealthPotion, "UIMessage.HealthPotion", "Tag for health potion UI message.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Message_ManaPotion, "UIMessage.ManaPotion", "Tag for mana potion UI message.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Message_HealthCrystal, "UIMessage.HealthCrystal", "Tag for health crystal UI message.");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Message_ManaCrystal, "UIMessage.ManaCrystal", "Tag for mana crystal UI message.");

