static inline bool _sc_land_translate_x(const sc_land *l, int *x) {
/* _sc_land_translate_x
   Translates the X coordinate indicated.  This will translate, taking into
   consideration the wall types.  It will return true if the coordinate
   could be translated to a "proper" value (or if the coordinate was
   off-screen on an unbounded boundary), caution the coordinate may NOT
   necessarily be in-bounds.  The function returns false if the coordinate
   could not be translated (coordinate was inside a wall), in which case X
   is set to the nearest valid X coordinate.  */

/* TEMP NOTE - The above documentation is INCORRECT.  The weapon WILL be
   in-bounds if the returned value is true.  If the coordinate is off-
   screen, false will be returned.  This needs audited and updated docs. */

   /* NOTE: changes to this code should also be made to the double code! */

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
