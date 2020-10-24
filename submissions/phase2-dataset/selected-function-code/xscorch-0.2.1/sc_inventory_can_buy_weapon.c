/* sc_inventory_can_buy_weapon
   Determine if this player can buy the specified weapon.  Returns true
   if the purchase can be made.  Budget is the amount of money allocated for
   the purchase, or may be -1 (player's total resources will be allowed into
   the calculation). */

   int count;        /* Number of weapons to buy */
   int cost;         /* Total cost to buy */

   /* Does the weapon exist? */
   if(info == NULL) return(false);

   /* Are we maxed out on inventory? */
   count = _sc_inventory_quantity(info->inventories[p->index], info->bundle);
   if(count == 0) return(false);

   /* Can we afford the weapon? */
   cost = _sc_inventory_purchase_price((sc_inventory_info *)info, count);
   if(cost > p->money) return(false);

   /* Is the weapon within our budget? */
   if(budget >= 0 && cost > budget) return(false);

   /* We can make this purchase. */
   return(true);

}
