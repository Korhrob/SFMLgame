#pragma once

#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int ScreenWidth = 1600;
const int ScreenHeight = 1200;

const float PlayerHeight = 55.0f;
const float PlayerWidth = 55.0f;
const float PlayerSpeed = 200.0f;

const float VampireHeight = 32.0f;
const float VampireWidth = 32.0f;
const float VampireSpeed = 100.0f;

const float ProjectileHeight = 32.0f;
const float ProjectileWidth = 32.0f;

const float WeaponHeight = 10.0f;
const float WeaponWidth = 100.0f;
const float WeaponActiveTime = 0.25f;

const float ArrowSpeed = 400.0f;
const float XbowArrowSpeed = 800.0f;
const float FireballSpeed = 250.0f;
const float SwordSwingSpeed = 300.0f;

const float CooldownUpgrade = 0.1f;

const float DefaultCooldowns[4] {
	2.0f, // Sword
	3.0f, // Fireball
	4.0f, // Bow
	5.0f  // Crossbow
};

const std::string name_sword = "Sword";
const std::string name_staff = "Staff";
const std::string name_bow = "Bow";
const std::string name_crossbow = "Crossbow";

const std::string desc_sword = "Swings at\nnearby enemy\n\nRange: Short\nProjectile Speed: Medium\nFire Rate: Fast";
const std::string desc_sword_add = "Increase attack rate by 10%";

const std::string name_sword_size = "Sword Size";
const std::string desc_sword_size = "Increase size by 10%";

const std::string desc_staff = "Shoots fireballs\nat nearest enemy\n\nRange: Medium\nProjectile Speed: Slow\nFire Rate: Medium";
const std::string desc_bow = "Shoots an arrow\nat furthest enemy\n\nRange: High\nProjectile Speed: Medium\nFire Rate: Slow";
const std::string desc_crossbow = "Shoots a bolt\nat closest enemy\n\nRange: Medium\nProjectile Speed: High\nFire Rate: Slowest";
