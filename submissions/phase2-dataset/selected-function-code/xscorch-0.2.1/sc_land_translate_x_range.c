bool sc_land_translate_x_range(const sc_land *l, int *x1, int *x2) {
/* _sc_land_translate_x_range
   Translates a range of virtual X coordinates s.t. at least part of the
   range is in the real coordinate space.  On bounded rooms, the interval
   will be truncated to the part that is inside the bound, true is returned
   if the result is nonempty.  On wraparound rooms, the interval is adjusted
   so that at least part of it is in the real coordinate range -- in this
   case, you will need to run sc_land_overlap_x() to see if part of the
   interval is truncated by the wrapping boundary (and get that part
   translated).  True is again, returned if the resulting interval is non-
   empty.  The interval is considered empty if x1 > x2.  */

   /* Sanity check */
   if(l == NULL || x1 == NULL || x2 == NULL) return(false);

   /* Interval is empty if x1 > x2. */
   if(*x1 > *x2) return(false);

   if(l->flags & SC_LAND_WRAP_X) {
      /* X boundaries wrap-around */
      while(*x1 >= l->width) {
         /* Adjust so x2 is in real */
         *x2 -= l->width;
         *x1 -= l->width;
      }
      while(*x2 < 0) {
         /* Adjust so x1 is in real */
         *x1 += l->width;
         *x2 += l->width;
      }
      /* Return success */
      return(true);
   } else {
      /* Bounded; truncate as needed */
      if(*x2 >= l->width) *x2 = l->width - 1;
      if(*x1 >= l->width) return(false);
      if(*x2 < 0) return(false);
      if(*x1 < 0) *x1 = 0;
      return(true);
   }

}
