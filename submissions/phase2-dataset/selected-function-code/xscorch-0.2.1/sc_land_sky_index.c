/* sc_land_sky_index
   Returns the list of sky gradient indices for the sky selected by
   the user.  Do not attempt to modify this array directly.  NULL is
   returned if an error occurred.  */

   /* Sanity check */
   if(c == NULL) return(NULL);

   /* Lookup the sky type. */
   switch(c->land->realsky) {
      case SC_SKY_NIGHT:
         return(c->colors->gradindex[SC_GRAD_NIGHT_SKY]);
      case SC_SKY_FIRE:
         return(c->colors->gradindex[SC_GRAD_FIRE_SKY]);
      case SC_SKY_RANDOM:
         return(NULL);
   } /* End switch */

   return(NULL);

}
