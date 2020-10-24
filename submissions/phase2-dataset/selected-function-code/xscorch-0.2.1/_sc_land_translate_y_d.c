/* _sc_land_translate_y_d
   This function is identical to above, but uses doubles.  */

   /* NOTE: changes to this code should also be made to the int code! */

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
