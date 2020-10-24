bool sc_inventory_sell_accessory(sc_player *p, sc_accessory_info *info) {
/* sc_inventory_sell_accessory
   Sells the specified accessory.  Returns true if the transaction was successful. */

   int count;        /* Number of accessories to buy */
   int cost;         /* Total cost to buy */

   /* Can we sell this accessory? */
   if(!sc_inventory_can_sell_accessory(p, info)) return(false);

   /* Get accessory sale information.. */
   count = info->inventories[p->index];
   if(count > info->bundle) count = info->bundle;
   cost = _sc_inventory_sale_price((sc_inventory_info *)info, count);

   /* TEMP HACK - This strips benefit unconditionally, and if
      another accessory also grants it, too bad.  Fix it! - JL
      NOTE - Thinking on this more, what will likely need to
      be done is loop though the accessories and look for any
      others that grant any of the same bits and then re-
      grant those bits in the state int.  hmm - JL */
   /* Remove any attributes granted by the accessory. */
   p->ac_state &= ~info->state;

   /* Sell the item */
   info->inventories[p->index] -= count;
   p->money += cost;

   /* Return success. */
   return(true);

}
