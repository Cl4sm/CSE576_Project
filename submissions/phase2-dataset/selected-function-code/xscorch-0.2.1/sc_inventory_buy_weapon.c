/* sc_inventory_buy_weapon
   Buys the specified weapon.  Returns true if the purchase was successful. */

   int count;        /* Number of weapons to buy */
   int cost;         /* Total cost to buy */

   /* Can we buy this weapon? */
   if(!sc_inventory_can_buy_weapon(p, info, SC_INVENTORY_INFINITE)) return(false);

   /* Get weapon purchase information.. */
   count = _sc_inventory_quantity(info->inventories[p->index], info->bundle);
   cost = _sc_inventory_purchase_price((sc_inventory_info *)info, count);

   /* Make the purchase */
   info->inventories[p->index] += count;
   p->money -= cost;

   /* Return success. */
   return(true);

}
