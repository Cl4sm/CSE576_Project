/* _sc_land_translate_x_d
   This function is identical to above, but uses doubles.  */

   /* NOTE: changes to this code should also be made to the int code! */

   if(l->flags & SC_LAND_WRAP_X) {
      /* X boundaries wrap-around */
      while(*x >= l->width) *x -= l->width;
      while(*x < 0) *x += l->width;
      return(true);
   } else if(l->flags & SC_LAND_WALL_X) {
      /* X boundaries are solid walls */
      if(*x >= l->width) {
         *x = l->width - 1;
         return(false);
      } else if(*x < 0) {
         *x = 0;
         return(false);
      } /* Is coordinate out-of-bounds? */
      return(true);
   } else {
      /* No boundaries at all. */
      if(*x >= l->width) return(false);
      if(*x < 0) return(false);
      return(true);
   } /* What is X boundary? */

}
