/* sc_inventory_can_sell_weapon
   Determine if this player can sell the specified weapon.
   Returns true if the sale can be made. */

   /* Does the weapon exist? */
   if(info == NULL) return(false);

   /* Are we min'd out on inventory? */
   if(info->inventories[p->index] <= 0)
      return(false);

   /* Is the weapon infinite? */
   if(SC_WEAPON_IS_INFINITE(info))
      return(false);

   /* We can make this transaction. */
   return(true);

}
