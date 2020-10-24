int sc_player_contact_trigger_count(const sc_config *c, const sc_player *p) {
/* sc_player_contact_trigger_count
   Return the number of contact triggers in this player's inventory. */

   sc_accessory_info *info, *cons = NULL, *perm = NULL;
   int count;

   /* Sanity check */
   if(c == NULL || p == NULL) return(false);

   /* Check to see if we have any contact triggers to use. */
   count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   for(; count > 0; --count) {
      /* Make sure the damn thing is a batch-o-contact-triggers */
      if(SC_ACCESSORY_IS_TRIGGER(info) && info->inventories[p->index] > 0) {
         if(SC_ACCESSORY_IS_CONSUMABLE(info)) cons = info;
         if(SC_ACCESSORY_IS_PERMANENT(info)) perm = info;
      }
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL);
   }

   /* Use permanent triggers if possible; otherwise try consumable ones. */
   if(perm != NULL) {
      /* Make use of permanent (distance?) trigger. */
      return(perm->inventories[p->index]);
   } else if(cons != NULL) {
      /* Consume a consumable (mechanical?) trigger. */
      return(cons->inventories[p->index]);
   } else {
      /* Failure; sorry. */
      return(0);
   }

}
