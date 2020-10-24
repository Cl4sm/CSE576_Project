/* sc_tank_profile_verify
   This function is responsible for making certain that the tank profile is 
   valid.  In particular, this function checks that the tank is base-aligned
   (the bottom row is not blank), and that the tank has a minimum area. The
   latter requirement keeps people from registering tanks that are one pixel
   in size, which is just far too evil.  Note, this function is tolerant of
   custom tank sizes.  True is returned if the tank is valid (or has been
   adjusted to a valid configuration), and false is returned on an error.  
   Caution, no sanity checks on profile pointer.  */

   const unsigned char *ps;   /* Used in profile reads and copy */
   unsigned char *pd;         /* Used in profile copy - destination */
   int lastrow;      /* Last row that wasn't completely clear */
   int height;       /* Total height of the profile */
   int width;        /* Total width of the profile */
   int area;         /* Total area of the profile (pixels) */
   int row;          /* Iterator: current row */
   int col;          /* Iterator: current column */

   /* Calculate the width, height of the profile; initialise area */
   width  = 1 + profile->radius * 2;
   height = 1 + profile->radius;
   area   = 0;

   /* Scan to determine the last nonclear row, and also calculate area */
   for(ps = profile->data, lastrow = 0, row = 0; row < height; ++row) {
      for(col = 0; col < width; ++col, ++ps) {
         if(*ps != SC_TANK_PROFILE_CLEAR) {
            /* Found a solid pixel */
            lastrow = row;
            ++area;
         } /* Found solid? */
      } /* Scanning columns */
   } /* Scanning rows */

   /* If last solid row was not base, then we will need to move the 
      profile downward so it is.  This is basically a memory copy. */
   if(lastrow < height - 1) {
      /* We need to move (height - 1 - lastrow) rows down. */
      ps = profile->data + (lastrow + 1) * width;  /* Source */
      pd = profile->data + height * width;         /* Destination */
      for(row = lastrow; row >= 0; --row) {
         for(col = width; col >= 0; --col) {
            /* Move this row down */
            --ps;
            --pd;
            *pd = *ps;
         } /* Moving column */
      } /* Moving rows down */
      
      /* This loop clears the top area, which is garbage now. */
      for(pd = profile->data, row = height - 1 - lastrow; row >= 0; --row) {
         for(col = width; col >= 0; ++pd, --col) {
            *pd = SC_TANK_PROFILE_CLEAR;
         } /* Clear columns */
      } /* Clear rows */
   } /* Had to move down profile? */

   /* Make sure tank isn't a single pixel. */   
   if(area < SC_TANK_PROFILE_MIN_AREA) {
      /* Too small! */
      fprintf(stderr, "Area of tank \"%s\" is too small, calculated %d < required %d\n",
              profile->name, area, SC_TANK_PROFILE_MIN_AREA);
      return(false);
   } /* Sanity check */

   /* Okay, the tank checks out! */
   return(true);

}
