/* _sc_land_translate_y
   Translates the Y coordinate indicated.  This will translate, taking into
   consideration the wall types.  It will return true if the coordinate
   could be translated to a "proper" value (or if the coordinate was
   off-screen on an unbounded boundary), caution the coordinate may NOT
   necessarily be in-bounds.  The function returns false if the coordinate
   could not be translated (coordinate was inside a wall), in which case X
   is set to the nearest valid Y coordinate.  */

/* TEMP NOTE - The above documentation is INCORRECT.  The weapon WILL be
   in-bounds if the returned value is true.  If the coordinate is off-
   screen, false will be returned.  This needs audited and updated docs. */

   /* NOTE: changes to this code should also be made to the double code! */

   if(l->flags & SC_LAND_CEILING) {
      /* Land has a ceiling */
      if(*y >= l->height) {
         *y = l->height - 1;
         return(false);
      } else if(*y < 0) {
         *y = 0;
         return(false);
      } /* Is Y invalid? */
      return(true);
   } else {
      /* Open skies */
      if(*y < 0) {
         *y = 0;
         return(false);
      } else if(*y >= l->height) {
         return(false);
      } /* Is Y invalid? */
      return(true);
   } /* End of switch */

}
