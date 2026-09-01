#include <OpenGL/Player/Inventory.h>
#include <assert.h>



Inventory::Inventory()
{}

Inventory::~Inventory()
{}

ItemStack& Inventory::getItemAtSlot(uint8_t slotIndex)
{
	assert(slotIndex < TOTAL_SLOT_COUNT && "Slot index out of bounds");
	return storageSlots[slotIndex];
}

void Inventory::addItemToInventory(const ItemStack& item, int index)
{
	storageSlots[index] = item;
}

void Inventory::deleteFromInventory(int index)
{
	storageSlots[index] = ItemStack();
}

void Inventory::removeCountFromItemAtSlot(uint8_t slotIndex, uint8_t count)
{
	ItemStack& currentSlot = storageSlots[slotIndex];
	currentSlot.count -= count;

	if (currentSlot.count == 0) {
		currentSlot.item = std::nullopt;
	}
}
