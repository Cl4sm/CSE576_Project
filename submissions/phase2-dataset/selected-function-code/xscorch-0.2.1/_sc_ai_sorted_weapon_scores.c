/* sc_ai_sorted_weapon_scores
   Impose an ordering on the weapons, based on scores loaded into *scores.  */

   int inner, outer;             /* Iterator variables... */
   int count = sc_weapon_count(wc, SC_WEAPON_LIMIT_ALL);

   /* Bubble-sort the scores */   
   for(outer = 0; outer < count; ++outer) {
      for(inner = 0; inner < outer; ++inner) {
         if(scores[inner] > scores[outer]) {
            scores[inner] += scores[outer];
            scores[outer] = scores[inner] - scores[outer];
            scores[inner] = scores[inner] - scores[outer];
            ordering[inner] += ordering[outer];
            ordering[outer] = ordering[inner] - ordering[outer];
            ordering[inner] = ordering[inner] - ordering[outer];
         }
      }
   }

}
