bool sc_inventory_buy_accessory(sc_player *p, sc_accessory_info *info) {
/* sc_inventory_buy_accessory
   Buys the specified accessory.  Returns true if the purchase was successful. */

   int count;        /* Number of accessories to buy */
   int cost;         /* Total cost to buy */

   /* Can we buy this accessory? */
   if(!sc_inventory_can_buy_accessory(p, info, SC_INVENTORY_INFINITE)) return(false);

   /* Get accessory purchase information.. */
   count = _sc_inventory_quantity(info->inventories[p->index], info->bundle);
   cost = _sc_inventory_purchase_price((sc_inventory_info *)info, count);

   /* Make the purchase */
   info->inventories[p->index] += count;
   p->money -= cost;

   /* Add any attributes granted by the accessory. */
   p->ac_state |= info->state;

   /* Return success. */
   return(true);

}
