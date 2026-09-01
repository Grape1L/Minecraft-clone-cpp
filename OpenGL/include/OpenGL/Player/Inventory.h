#pragma once

#include <array>
#include <cstdint>
#include <OpenGL/Blocks/Block.h>
#include <OpenGL/Weapons/Weapon.h>
#include <variant>
#include <optional>


using ItemType = std::variant<BlockType, Weapon::Weapon>;

struct ItemStack {
	std::optional<ItemType> item = std::nullopt;
	uint8_t count = 0;


	const bool isBlock() {
		if (!item.has_value()) return false;
		return std::holds_alternative<BlockType>(*item);
	}

	const bool isWeapon() {
		if (!item.has_value()) return false;
		return std::holds_alternative<Weapon::Weapon>(*item);
	}

	BlockType* getBlock() {
		if (!item) return nullptr;
		return std::get_if<BlockType>(&*item);
	}

	Weapon::Weapon* getWeapon() {
		if (!item) return nullptr;
		return std::get_if<Weapon::Weapon>(&*item);
	}
};

class Inventory {
public:
	Inventory();
	~Inventory();

public:
	static constexpr uint8_t STORAGE_SLOT_COUNT = 36;
	static constexpr uint8_t HOTBAR_SLOT_COUNT = 9;
	static constexpr uint8_t TOTAL_SLOT_COUNT = STORAGE_SLOT_COUNT + HOTBAR_SLOT_COUNT;

public:
	uint8_t currentSlot = 0;

public:
	ItemStack& getItemAtSlot(uint8_t slotIndex);
	void addItemToInventory(const ItemStack& item, int index);
	void deleteFromInventory(int index);
	void removeCountFromItemAtSlot(uint8_t slotIndex, uint8_t count);

private:
	std::array<ItemStack, TOTAL_SLOT_COUNT> storageSlots = {};
};