/* sc_ai_select_last_weapon
   Selects the last (highest-index) weapon available to the AI.  The naive
   AI's make the assumption that weapons further down the inventory list
   must clearly be better.  */

   sc_weapon_info *info;
   int count;

   assert(c != NULL && p != NULL);

   count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);

   for(/* No initializer */; count > 0; --count) {
      if(info->inventories[p->index] > 0) {
         /* We found a weapon.. */
         if(SC_AI_DEBUG_SELECT) {
            printf("   AI %s selected weapon \"%s\"\n", sc_ai_name(p->aitype), info->name);
         }
         sc_player_set_weapon(c, p, info);
         return;
      } /* Do we have this weapon? */
   } /* Looking for a weapon... */

   /* Well, heh, if we get here we have no weapons -- should be impossible. */
   if(SC_AI_DEBUG_SELECT) {
      printf("   AI %s has no weapons to fire!\n", sc_ai_name(p->aitype));
   }

}
