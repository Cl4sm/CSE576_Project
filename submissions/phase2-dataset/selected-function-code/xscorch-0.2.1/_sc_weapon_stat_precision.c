/* _sc_weapon_stat_precision
   Find a rough estimator of a weapon's precision.
   Later on we might have a wind shear value, and/or a
   weapon eccentricity value, which would also contribute.
   During most of this code precision is actually inverted... */

   double precision;             /* Estimate of weapon precision */
   const sc_weapon_info *child;  /* Info on child weapon(s) */
   int numchildren;              /* Number of children for phoenix weapon */
   int count;                    /* A counter for frog type phoenix weapons */

   /* Horizonal (i.e. not area) space covered, usually 2 * radius.
      If liquid (napalm), the area of flooding is the horizonal space. */
   if(SC_WEAPON_IS_LIQUID(info))
      precision = info->liquid;
   else
      precision = 2 * info->radius;

   /* Triple Turret weapons are a third as precise, if we have such a turret. */
   if(triple && SC_WEAPON_IS_TRIPLE(info))
      precision *= 2;

   /* Phoenix weapons have special precision rules. */
   if(SC_WEAPON_IS_PHOENIX(info)) {
      if(!sc_phoenix_verify(wc, info)) {
         /* This means somebody snuck in a bad child pointer on a
            phoenix weapon.  It shouldn't happen, but if it does, we
            promise to return a negative yield.  AIs are advised to
            select another weapon as this one may cause the game to
            segfault or die and nobody wants that! */
         fprintf(stderr, "Bad phoenix weapon: %s.  Children loop or do not exist.\n", info->name);
         return(precision ? -abs(precision) : -1);
      }
      child = sc_weapon_lookup(wc, SC_PHOENIX_CHILD_TYPE(info), SC_WEAPON_LIMIT_NONE);
      numchildren = SC_PHOENIX_CHILD_COUNT(info);
      if(SC_PHOENIX_IS_CHAIN(info)) {
         /* Allow the children to vote on their precisions. */
         for(count = 1; count <= numchildren; ++count)
            precision += pow(SC_PHOENIX_INCREASE_FACTOR, count) * 100 / _sc_weapon_stat_precision(wc, child, triple);
      } else if(SC_PHOENIX_IS_CONVERT(info)) {
         /* Conversions could be more precise, but for now AI's don't know about them. */
         precision *= 100;  /* TEMP - Arbitrary! - JL */
      } else if(SC_PHOENIX_IS_SCATTER(info)) {
         /* Scattering doesn't detonate the original weapon. */
         if(child != info) precision = numchildren * 100 / _sc_weapon_stat_precision(wc, child, triple);
         else              precision = numchildren * precision;
         /* As a rule, scattering is very imprecise, and we hit its precision value hard here. */
         precision *= SC_PHOENIX_SCATTER(info);
      } else if(SC_PHOENIX_IS_SPIDER(info)) {
         /* Spiders detonate the original weapon in the center, and on average 75% of their children */
         if(child != info) precision += 0.75 * numchildren * 100 / _sc_weapon_stat_precision(wc, child, triple);
         else              precision += 0.75 * numchildren * precision;
         /* Spiders are also docked for their imprecise nature. */
         precision *= 2 * SC_PHOENIX_SCATTER(info);
      } else if(SC_PHOENIX_IS_DETONATE(info)) {
         /* We do nothing with detonation requests; they have their face value precision. */
      }
   }

   /* Invert the precision so bigger is better, and normalize a bit.
      This will give a Missile a precision of 1.25, by the way.
      If the weapon is completely imprecise, it scores a zero.
      Likewise, if it doesn't really explode, it gets a zero. */
   precision = (precision && info->force) ? (100 / precision) : 0;

   return(precision);

}
