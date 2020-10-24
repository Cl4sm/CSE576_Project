/* sc_player_use_contact_trigger
   Try to use a contact trigger.  If successful, return true. */

   sc_accessory_info *info, *cons = NULL, *perm = NULL;
   int count, totrig = 0;

   /* Sanity check */
   if(c == NULL || p == NULL) return(false);

   /* Are they enabled? */
   if(!p->contacttriggers) return(false);

   /* Check to see if we have any contact triggers to use. */
   count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   for(; count > 0; --count) {
      /* Make sure the damn thing is a batch-o-contact-triggers */
      if(SC_ACCESSORY_IS_TRIGGER(info) && info->inventories[p->index] > 0) {
         ++totrig;
         if(SC_ACCESSORY_IS_CONSUMABLE(info))
            cons = info;
         if(SC_ACCESSORY_IS_PERMANENT(info))
            perm = info;
      }
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL);
   }

   /* Use permanent triggers if possible; otherwise try consumable ones. */
   if(perm != NULL) {
      /* Make use of permanent (distance?) trigger. */
      return(true);
   } else if(cons != NULL) {
      /* Consume a consumable (mechanical?) trigger. */
      --cons->inventories[p->index];
      if(totrig <= 1 && info->inventories[p->index] <= 0) {
         p->ac_state &= ~SC_ACCESSORY_STATE_TRIGGER;
         sc_player_set_contact_triggers(c, p, false);
      }
      return(true);
   } else {
      /* Failure; sorry. */
      return(false);
   }

}
