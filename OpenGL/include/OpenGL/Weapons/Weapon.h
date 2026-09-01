#pragma once

namespace Weapon {
	enum class WeaponType {
		Air, Sword, Gun
	};

	struct Weapon {
		WeaponType type = WeaponType::Air;
		float range;
		float damage;
		float durability;
	};


	inline Weapon woodenSword{ WeaponType::Sword, 4.5f, 10, 100 };

	inline Weapon pistol{ WeaponType::Gun, 1000.0f, 50, 0 };
};