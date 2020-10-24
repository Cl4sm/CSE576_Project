/* sc_inventory_sell_weapon
   Sells the specified weapon.  Returns true if the transaction was successful. */

   int count;        /* Number of weapons to buy */
   int cost;         /* Total cost to buy */

   /* Can we sell this weapon? */
   if(!sc_inventory_can_sell_weapon(p, info)) return(false);

   /* Get weapon sale information.. */
   count = info->inventories[p->index];
   if(count > info->bundle) count = info->bundle;
   cost = _sc_inventory_sale_price((sc_inventory_info *)info, count);

   /* Sell the item */
   info->inventories[p->index] -= count;
   p->money += cost;

   /* Return success. */
   return(true);

}
