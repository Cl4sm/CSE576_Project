bool sc_inventory_can_sell_accessory(const sc_player *p, const sc_accessory_info *info) {
/* sc_inventory_can_sell_accessory
   Determine if this player can sell the specified accessory.
   Returns true if the sale can be made. */

   /* Does the accessory exist? */
   if(info == NULL) return(false);

   /* Are we min'd out on inventory? */
   if(info->inventories[p->index] <= 0)
      return(false);

   /* Is the accessory infinite? */
   if(SC_ACCESSORY_IS_INFINITE(info))
      return(false);

   /* We can make this transaction. */
   return(true);

}
