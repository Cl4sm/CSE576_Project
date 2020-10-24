int sc_land_min_height_around(const sc_land *l, int x, int y0, int w) {
/* sc_land_min_height_around
   Returns the MINIMUM height around an interval.  Similar to the
   above function in approach.  */

   int height;          /* Maximum height of land found */
   int ch;              /* Height of current column */
   int cx;              /* Current X coordinate (translated) */
   int x1;              /* First virtual X to check */
   int x2;              /* Last virtual X to check */

   /* Sanity check */
   if(l == NULL) return(0);

   /* Calculate the virtual interval */
   x1 = x - w;
   x2 = x + w;

   /* Make sure the maximum height is bounded */
   if(y0 >= l->height) y0 = l->height - 1;

   /* Start searching... */
   height = 0;
   for(x = x1; x <= x2; ++x) {
      /* Attempt to translate X to a real coordinate */
      cx = x;
      if(_sc_land_translate_x(l, &cx)) {
         /* Check the height for this column */
         ch = _sc_land_height(l, cx, y0);
         if(ch < height) height = ch;
      } /* End translation valid? */
   } /* End loop through interval */

   /* Return the minimum height found. */
   return(height);

}
