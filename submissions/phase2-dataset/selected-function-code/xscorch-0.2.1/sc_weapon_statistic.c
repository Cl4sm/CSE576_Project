                           const sc_player *p, sc_weapon_stat statistic) {
/* sc_weapon_statistic
   Calculate various weapon statistics used in AI purchasing.
   The sc_player pointer is optional (can be NULL, in which case you get defaults). */

   bool triple;
   double result;

   /* Can't allow people to pass us bad info, now can we...? */
   if(wc == NULL || info == NULL)
      return(0);

   /* We set triple to a bool value that is true if we have a triple turret. */
   triple = p && (p->ac_state & SC_ACCESSORY_STATE_TRIPLE);

   switch(statistic) {
      case SC_WEAPON_STAT_PRICE:
         if(info->bundle)
            result = info->price / info->bundle;
         else
            result = 0;
         break;
      case SC_WEAPON_STAT_YIELD:
         result = _sc_weapon_stat_yield(wc, info, triple);
         break;
      case SC_WEAPON_STAT_PRECISION:
         result = _sc_weapon_stat_precision(wc, info, triple);
         break;
      case SC_WEAPON_STAT_ECON_YIELD:
         if(info->price)
            result = _sc_weapon_stat_yield(wc, info, triple) * info->bundle / info->price;
         else
            result = _sc_weapon_stat_yield(wc, info, triple) * info->bundle / SC_INVENTORY_CHEAPO_FACTOR;
         break;
      case SC_WEAPON_STAT_ECON_PRECISION:
         if(info->price)
            result = _sc_weapon_stat_precision(wc, info, triple) * info->bundle / info->price;
         else
            result = _sc_weapon_stat_precision(wc, info, triple) * info->bundle / SC_INVENTORY_CHEAPO_FACTOR;
         break;
      case SC_WEAPON_STAT_PRECISION_YIELD:
         result = _sc_weapon_stat_precision(wc, info, triple) * _sc_weapon_stat_yield(wc, info, triple);
         break;
      default:
         printf("Warning - someone asked for a weapon statistic we don't know how to calculate!\n");
         result = 0;
   }

   return(result);

}
