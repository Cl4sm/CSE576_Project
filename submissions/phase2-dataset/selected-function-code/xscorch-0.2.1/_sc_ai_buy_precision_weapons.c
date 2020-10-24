static bool _sc_ai_buy_precision_weapons(const sc_config *c, sc_player *p, int *budget) {
/* sc_ai_buy_precision_weapons
   Buy the weapons yielding high power and precision.  This is for use
   by AI players who know exactly how to aim and exactly what/who they
   want to destroy.  Such a player can take out her neighbor without
   also destroying herself as one might expect of AI players that make
   use of _sc_ai_buy_weapons_by_score(). */

   sc_weapon_info *info;
   int count = sc_weapon_count(c->weapons, SC_WEAPON_LIMIT_ALL);
   /* Sorted indices, based on score. */
   int *ordering = (int *)malloc(sizeof(int) * count);
   /* Sorted scores corresponding to ordering */
   double *scores = (double *)malloc(sizeof(double) * count);
   int i; /* Iterator */
   bool result;

   if(ordering == NULL || scores == NULL) {
      fprintf(stderr, "Malloc error in sc_ai_buy_precision_weapons!\n");
      free(ordering);
      free(scores);
      return(false);
   }

   /* Calculate scores, and set initial ordering */
   info = sc_weapon_first(c->weapons, SC_WEAPON_LIMIT_ALL | SC_WEAPON_SCAN_FORWARD);
   for(i = 0; i < count; ++i) {
      ordering[i] = info->ident;
      scores[i] = sc_weapon_statistic(c->weapons, info, p, SC_WEAPON_STAT_PRECISION_YIELD);
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
