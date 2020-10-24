bool sc_inventory_can_buy_accessory(const sc_player *p, const sc_accessory_info *info, int budget) {
/* sc_inventory_can_buy_accessory
   Determine if this player can buy the specified accessory.  Returns true
   if the purchase can be made.  Budget is the amount of money allocated for
   the purchase, or may be -1 (player's total resources will be allowed into
   the calculation). */

   int count;        /* Number of accessories to buy */
   int cost;         /* Total cost to buy */

   /* Does the accessory exist? */
   if(info == NULL) return(false);

   /* Is the accessory a one-time buy we already own? */
   if(SC_ACCESSORY_IS_PERMANENT(info) && info->inventories[p->index] > 0) return(false);

   /* Are we maxed out on inventory? */
   count = _sc_inventory_quantity(info->inventories[p->index], info->bundle);
   if(count == 0) return(false);

   /* Can we afford the accessory? */
   cost = _sc_inventory_purchase_price((sc_inventory_info *)info, count);
   if(cost > p->money) return(false);

   /* Is the accessory in our budget? */
   if(budget >= 0 && cost > budget) return(false);

   /* We can make this purchase. */
   return(true);

}
