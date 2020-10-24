sc_phoenix_result sc_phoenix(int locate, const sc_config *c, sc_weapon **wp, sc_explosion **e) {
/* sc_phoenix_result
   Check if a weapon is phoenix class and if so call the requested action on it.
   It is important to *NOTE* that when these subfunctions return, the original
   wp pointer is left intact.  It MUST BE DELETED by the calling function if the
   return value is either SC_PHOENIX_DETONATE or SC_PHOENIX_SIZZLE.  In certain
   cases like spider explosions, detonations will be created by the subfunctions.
   It is up to the caller to verify that the weapon is intended to be modified
   at the phoenix location it is calling from. */

   sc_phoenix_result result = SC_PHOENIX_NO_ACTION;

   /* Run the appropriate phoenix function(s); NOTE that although this is not
      an either/or operation, strange things may happen if you take advantage
      of this fact.  We simply haven't tested what will occur. */
   if(SC_PHOENIX_IS_DETONATE((*wp)->weaponinfo)) {
      result = _sc_phoenix_detonate(locate, c, wp, e);
      if(result == SC_PHOENIX_DETONATE || result == SC_PHOENIX_FAILURE) goto out;
   }
   if(SC_PHOENIX_CHILD_TYPE((*wp)->weaponinfo)  <= 0 ||
      SC_PHOENIX_CHILD_COUNT((*wp)->weaponinfo) <= 0) {
      /* All of the remaining operations require children... */
      goto out;
   }
   if(SC_PHOENIX_IS_CONVERT((*wp)->weaponinfo)) {
      result = _sc_phoenix_convert(locate, c, wp, e);
      if(result == SC_PHOENIX_RESET || result == SC_PHOENIX_FAILURE) goto out;
   }
   if(SC_PHOENIX_IS_CHAIN((*wp)->weaponinfo)) {
      result = _sc_phoenix_chain(locate, c, wp, e);
      if(result == SC_PHOENIX_DETONATE || result == SC_PHOENIX_FAILURE) goto out;
   }
   if(SC_PHOENIX_IS_SPIDER((*wp)->weaponinfo)) {
      result = _sc_phoenix_spider(locate, c, wp, e);
      if(result == SC_PHOENIX_DETONATE || result == SC_PHOENIX_FAILURE) goto out;
   }
   if(SC_PHOENIX_IS_SCATTER((*wp)->weaponinfo)) {
      result = _sc_phoenix_scatter(locate, c, wp, e);
      if(result == SC_PHOENIX_DETONATE || result == SC_PHOENIX_FAILURE) goto out;
   }

   out:
   return(result);

}
