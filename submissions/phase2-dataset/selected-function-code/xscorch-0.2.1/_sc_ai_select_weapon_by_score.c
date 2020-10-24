/* sc_ai_select_weapon_by_score
   Selects the highest-ranked weapon available to the AI.  This is slightly
   better, but we might end up selecting a very powerful weapon to kill only
   a single tank.  */

   sc_weapon_info *info;        /* Weapon tracking */
   sc_weapon_info *maxinfo;     /* Weapon index of best weapon */
   sc_weapon_info *nextinfo;    /* Weapon index of second-best */
   int maxscore;     /* Score of best weapon so far */
   int nextscore;    /* Score of second best weapon */
   int score;        /* Score of current weapon */
   int count;        /* Iterator */

   assert(c != NULL && p != NULL);

   /* Setup and iterate */   
   count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
   maxinfo = NULL;
   nextinfo= NULL;
   maxscore = 0;
   nextscore= -1;

   for(/* No initializer */; count > 0; --count) {
      if(info->inventories[p->index] > 0) {
         /* We have some of this weapon; is it ideal? */
         score = sc_weapon_statistic(c->weapons, info, p, SC_WEAPON_STAT_YIELD);
         if(score > maxscore) {
            /* This weapon is ideal (so far) */
            nextinfo = maxinfo;
            nextscore= maxscore;
            maxinfo  = info;
            maxscore = score;
         } else if(score > nextscore) {
            /* weapon is second-best (so far) */
            nextinfo = info;
            nextscore= score;
         } /* Does this weapon have a better score? */
      } /* Do we have this weapon in inventory? */
      info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_REVERSE);
   } /* Loop through all weapons. */

   /* Set new weapon to one of the "best" found. */
   if(nextinfo != NULL && game_drand() < (double)nextscore / (nextscore + maxscore)) {
      /* Probabilistically select "next best" */
      maxinfo = nextinfo;
   }

   /* We have only one option now */
   if(maxinfo != NULL) {
      sc_player_set_weapon(c, p, maxinfo);
      if(SC_AI_DEBUG_SELECT) {
         printf("   AI %s selected weapon \"%s\"\n", sc_ai_name(p->aitype), maxinfo->name);
      }
   } else if(SC_AI_DEBUG_SELECT) {
      printf("   AI %s has no weapons to fire!\n", sc_ai_name(p->aitype));
   }

}
