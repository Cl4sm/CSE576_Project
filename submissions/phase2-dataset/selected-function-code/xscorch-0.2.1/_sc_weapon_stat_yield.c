static double _sc_weapon_stat_yield(const sc_weapon_config *wc, const sc_weapon_info *info, bool triple) {
/* _sc_weapon_stat_yield
   Find an upper bound yield for a weapon.  */

   double yield;                 /* Total weapon yield */
   const sc_weapon_info *child;  /* Info on child weapon(s) */
   int numchildren;              /* Number of children for phoenix weapon */
   int count;                    /* A counter for frog type phoenix weapons */

   /* Yield from self */
   yield = SQR(info->radius) * info->force;

   /* If liquid (napalm), must multiply by potential area of flooding
      -- this should scale hot napalm to near the vicinity of a nuke
      (but less), which is reasonable, considering both can take down
      large tanks.  */
   if(SC_WEAPON_IS_LIQUID(info)) yield *= info->liquid;

   /* Triple Turret weapons get three times the kaboom, but only if we have such a turret. */
   if(triple && SC_WEAPON_IS_TRIPLE(info))
      yield *= 3;

   /* Phoenix weapons have special yield rules. */
   if(SC_WEAPON_IS_PHOENIX(info)) {
      if(!sc_phoenix_verify(wc, info)) {
         /* This means somebody snuck in a bad child pointer on a
            phoenix weapon.  It shouldn't happen, but if it does, we
            promise to return a negative yield.  AIs are advised to
            select another weapon as this one may cause the game to
            segfault or die and nobody wants that! */
         fprintf(stderr, "Bad phoenix weapon: %s.  Children loop or do not exist.\n", info->name);
         return(yield ? -abs(yield) : -1);
      }
      child = sc_weapon_lookup(wc, SC_PHOENIX_CHILD_TYPE(info), SC_WEAPON_LIMIT_NONE);
      numchildren = SC_PHOENIX_CHILD_COUNT(info);
      if(SC_PHOENIX_IS_CHAIN(info)) {
         /* Each child is made smaller by a constant factor, _including_ the first child */
         for(count = 1; count <= numchildren; ++count)
            yield += _sc_weapon_stat_yield(wc, child, triple) * pow(SC_PHOENIX_INCREASE_FACTOR, count);
      } else if(SC_PHOENIX_IS_CONVERT(info)) {
         /* conversions probably aren't well represented here */
         if(child != info) yield = _sc_weapon_stat_yield(wc, child, triple);
      } else if(SC_PHOENIX_IS_SCATTER(info)) {
         /* scattering doesn't detonate the original weapon */
         if(child != info) yield = numchildren * _sc_weapon_stat_yield(wc, child, triple);
         else              yield = numchildren * yield;
         /* as a policy, scattering takes a slight bonus for "effectiveness" */
         yield *= (SC_PHOENIX_YIELD_SCATTER_SCALE + SC_PHOENIX_SCATTER(info)) / SC_PHOENIX_YIELD_SCATTER_SCALE;
      } else if(SC_PHOENIX_IS_SPIDER(info)) {
         /* spiders detonate the original weapon in the center, and on average 75% of their children */
         if(child != info) yield += 0.75 * numchildren * _sc_weapon_stat_yield(wc, child, triple);
         else              yield += 0.75 * numchildren * yield;
      } else if(SC_PHOENIX_IS_DETONATE(info)) {
         /* We do nothing with detonation requests; they have their face value yield. */
      }
   }

   return(yield);

}
