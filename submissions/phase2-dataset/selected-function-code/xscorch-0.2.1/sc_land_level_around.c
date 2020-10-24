void sc_land_level_around(const sc_config *c, sc_land *l, int x, int w, int ht) {
/* sc_land_level_around
   This function levels the land to a uniform height, ht.  This function
   levels the virtual interval [x - w, x + w], inclusive.  No values are
   returned, this function silently exits on failure.  This function is
   useful for leveling a platform to place a tank on.  */

   int cx;           /* Translated current X coordinate. */
   int x1;           /* Left virtual X coordiante */
   int x2;           /* Right virtual X coordinate */

   /* Sanity checks */
   if(c == NULL || l == NULL) return;

   /* Calculate the left, right bounds of the interval */
   x1 = x - w;
   x2 = x + w;

   /* Level the columns in the interval. */
   cx = 0;
   for(x = x1; cx <= x2; ++x) {
      cx = x;
      if(_sc_land_translate_x(l, &cx)) {
         /* Coordinate translated, level it. */
         _sc_land_level(c, l, cx, ht);
      } /* End translation valid? */
   } /* End interval loop */

}
