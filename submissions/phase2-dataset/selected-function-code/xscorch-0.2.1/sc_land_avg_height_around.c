/* sc_land_avg_height_around
   Returns the average height of the land over a virtual interval.
   Similar to the previous two functions in approach.  */

   int count;           /* Number of heights found */
   int sum;             /* Sum of heights found */
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
   sum = 0;
   count = 0;
   for(x = x1; x <= x2; ++x) {
      /* Attempt to translate X to a real coordinate */
      cx = x;
      if(_sc_land_translate_x(l, &cx)) {
         /* Add the height for this column */
         sum += _sc_land_height(l, cx, y0);
         ++count;
      } /* End translation valid? */
   } /* End loop through interval */

   /* Return the average of heights found. */
   return(sum / count);

}
