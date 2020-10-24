/* sc_ai_buy_weapons_by_score
   Buy the weapons yielding the best destructive power.  This is a great
   buying strategy for players that just want to annihilate the entire
   playing field.  Coupled with agressive, this can be quite deadly!  */

   sc_weapon_info *info;
   int count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   /* Sorted indices, based on score. */
   int *ordering = (int *)malloc(sizeof(int) * count);
   /* Sorted scores corresponding to ordering */
   double *scores = (double *)malloc(sizeof(double) * count);
   int i; /* Iterator */
   bool result;

   if(ordering == NULL || scores == NULL) {
      fprintf(stderr, "Malloc error in sc_ai_buy_weapons_by_score!\n");
      free(ordering);
      free(scores);
      return(false);
   }

   /* Calculate scores, and set initial ordering */
   info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_FORWARD);
   for(i = 0; i < count; ++i) {
      ordering[i] = info->ident;
      scores[i] = sc_weapon_statistic(c->weapons, info, p, SC_WEAPON_STAT_YIELD);
      info = sc_weapon_next(c->weapons, info, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_FORWARD);
   }

   /* Bubble-sort the scores */   
   _sc_ai_sorted_weapon_scores(c->weapons, ordering, scores);

   /* Iterate through, beginning with the "best" weapons. */
   result = _sc_ai_buy_weapons_from_list(c, p, budget, ordering);

   free(ordering);
   free(scores);
   return(result);

}
