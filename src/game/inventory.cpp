#include "inventory.h"


void Inventory::add(ItemType itemType)
{
   InventoryItem item;
   item.mType = itemType;

   mItems.push_back(item);
}


void Inventory::clear()
{
   mItems.clear();
}


const std::vector<InventoryItem>& Inventory::getItems() const
{
   return mItems;
}


void Inventory::resetKeys()
{
   mItems.erase(
      std::remove_if(
         mItems.begin(),
         mItems.end(),
         [](auto& item) -> bool
         {
            if (
                  item.mType == ItemType::KeyBlue
               || item.mType == ItemType::KeyGreen
               || item.mType == ItemType::KeyRed
               || item.mType == ItemType::KeyYellow
            )
            {
               return true;
            }

            return false;
         }
      ),
      mItems.end()
   );
}


void Inventory::giveAllKeys()
{
   add(ItemType::KeyRed);
   add(ItemType::KeyYellow);
   add(ItemType::KeyBlue);
   add(ItemType::KeyGreen);
}


bool Inventory::hasInventoryItem(ItemType itemType) const
{
   if (itemType == ItemType::Invalid)
   {
      return true;
   }

   const auto& it = std::find_if(std::begin(mItems), std::end(mItems), [itemType](auto item) {
         return (item.mType == itemType);
      }
   );

   return it != mItems.end();
}


