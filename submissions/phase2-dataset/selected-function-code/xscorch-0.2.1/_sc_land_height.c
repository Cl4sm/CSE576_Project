/* sc_land_height
   This is an internal function that assumes x, y0 are real coordiantes.
   This function will return a maximum of y0; if you want to scan the entire
   range, give the screen height for y0.  */

   const int *lp;       /* Pointer into land structure. */

   /* Get the land pointer for this column. */
   lp = SC_LAND_XY(l, x, y0);
   while(y0 > 0) {
      if(!SC_LAND_IS_SKY(*lp)) return(y0 + 1);
      --lp;
      --y0;
   } /* End translation valid? */

   /* No land here; we hit the ground. */
   return(0);

}
