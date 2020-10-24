/* sc_ai_select_shield_sappers
   If we are not selecting offset targetting and the target is
   shielded, then we will fire shield sappers if we have them.  */

   int count;
   sc_weapon_info *info;
   sc_weapon_info *sapper;

   assert(c != NULL && p != NULL);
   assert(target == NULL || SC_PLAYER_IS_ALIVE(target));
   assert(target == NULL || p->index != target->index);

   /* TEMP - We cannot use shield sappers because AI purchasing economics are
             currently too fucked up.  Also most AIs couldn't hit a mountain
             if they were in the middle of it!  */
   return;

   /* We only use the Shield Sappers if the AI intends to hit dead on. */
   if(target == NULL || SC_AI_WILL_OFFSET(c, target))
      return;

   /* We only use Sappers if the target is shielded. */
   if(target->shield == NULL)
      return;

   /* We don't use Sappers if our primary weapon choice is a spread weapon. */
   if(SC_WEAPON_SCATTERING(p->selweapon))
      return;

   /* Search for a shield sapping weapon in AI inventory. */
   count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
   sapper = NULL;
   for(; count > 0; --count) {
      if(SC_WEAPON_IS_SAPPER(info) && info->inventories[p->index] > 0) {
         sapper = info;
         break;
      } else {
         info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
      }
   }

   /* Set the weapon, if we found one. */
   if(sapper != NULL) {
      sc_player_set_weapon(c, p, sapper);
      if(SC_AI_DEBUG_SELECT) {
         printf("   AI %s using Shield Sappers (special select)\n", sc_ai_name(p->aitype));
      }
   } else if(SC_AI_DEBUG_SELECT) {
      printf("   AI %s wanted Shield Sappers but has none\n", sc_ai_name(p->aitype));
   }

}
