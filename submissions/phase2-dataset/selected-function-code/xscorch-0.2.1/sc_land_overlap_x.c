bool sc_land_overlap_x(const sc_land *l, int *x1, int *x2) {
/* _sc_land_overlap_x
   Determines if the X range described by [x1, x2] would overlap a wrapping
   boundary.  This function behaves rather oddly; usually the center of the
   interval was a valid real coordinate, and x1, x2 are obtained by
   calculating some fixed offset from that valid real coordinate.  If an
   overlap on a boundary is detected, then the interval is modified to
   indicate the interval that was `wrapped', i.e. the interval part that was
   excluded because it was not in the valid real range.  False is returned
   if the boundaries aren't wrapping, or if no translation is required (x1,
   x2 are in the valid range for real coordinates.  This function may have
   odd semantics if at least one of the coordinates was not anchored in the
   valid real range.  The semantics should probably be documented better
   than this.  This is primarily used to determine the range of update for
   drawing functions, where the semantics are odd but understood.

   CAUTION: the interval must not be wider than the screen.
   CAUTION: at least part of the interval must be valid real coords.

   You probably only want to give this function intervals that have been
   through the sc_land_translate_x_range() function, first!!!  */

   /* Sanity check */
   if(l == NULL || x1 == NULL || x2 == NULL) return(false);

   if(l->flags & SC_LAND_WRAP_X) {
      /* X boundaries wrap-around */
      if(*x1 < 0) {
         /* Wrap on -x side, return interval part that was truncated
            because it wrapped to the right-hand-side */
         *x1 += l->width;
         *x2  = l->width - 1;
         return(true);
      } else if(*x2 >= l->width) {
         /* Wrap on +x side, return interval part that was truncated
            because it wrapped to the left-hand-side */
         *x2 -= l->width;
         *x1  = 0;
         return(true);
      }
   }
   return(false);

}
