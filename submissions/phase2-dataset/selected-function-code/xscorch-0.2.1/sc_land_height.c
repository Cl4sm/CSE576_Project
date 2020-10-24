int sc_land_height(const sc_land *l, int x, int y0) {
/* sc_land_height
   This function returns the height of the land in virtual column x.  The
   height of the lowest non-land pixel will be returned by this function.
   This function only considers land below and at the height y0, so the
   range of return values is from 0 to y0 + 1.  To check the height of an
   entire column, give the screen height for y0.  Caution: y0 is a height,
   not a virtual coordinate.  */

   /* Sanity check */
   if(l == NULL) return(0);

   /* Make sure the height is properly bounded. */
   if(y0 >= l->height) y0 = l->height - 1;

   /* translate the X coordinate to a real coordinate. */
   if(!_sc_land_translate_x(l, &x)) return(0);
   return(_sc_land_height(l, x, y0));

}
