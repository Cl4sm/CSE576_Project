static bool _sc_ai_buy_best_shields(const sc_config *c, sc_player *p, int *budget) {
/* sc_ai_buy_best_shields
   Buy the best shields on the list that we can afford.  We will
   want to sort the shields out before deciding which to buy.  */

   sc_accessory_info *info;
   int count = sc_accessory_count(c->accessories, SC_ACCESSORY_LIMIT_ALL);
   /* Sorted indices, based on score. */
   int *ordering = (int *)malloc(sizeof(int) * count);
   /* Sorted scores corresponding to ordering */
   int *scores = (int *)malloc(sizeof(int) * count);
   int i; /* Iterator */
   bool result;

   if(ordering == NULL || scores == NULL) {
      fprintf(stderr, "Malloc error in sc_ai_buy_best_shields!\n");
      free(ordering);
      free(scores);
      return(false);
   }

   /* Calculate scores, and set initial ordering */
   info = sc_accessory_first(c->accessories, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   for(i = 0; i < count; ++i) {
      ordering[i] = info->ident;
      /* TEMP HACK - This should call a function in sgame/saccessory.c like the weapon version does. -JL */
      /* TEMP NOTE ABOUT MY TEMP HACK - Actually maybe sgame/sshield.c would make more sense, heh... */
      scores[i] = info->shield;  /* = sc_accessory_yield(c->accessories, info); */
      info = sc_accessory_next(c->accessories, info, SC_ACCESSORY_LIMIT_ALL | SC_ACCESSORY_SCAN_FORWARD);
   }

   /* TEMP NOTE NO. 2 - Ideally we should decide what's best based on type as well...
                        For example, a spreader probably prefers standard shielding.  ? */
   /* Bubble-sort the scores */
   _sc_ai_sorted_accessory_scores(c->accessories, ordering, scores);

   /* Iterate through, beginning with the "best" shields. */
   result = _sc_ai_buy_shields_from_list(c, p, budget, ordering);

   free(ordering);
   free(scores);
   return(result);

}
