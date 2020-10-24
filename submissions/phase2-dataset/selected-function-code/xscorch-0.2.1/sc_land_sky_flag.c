/* sc_land_sky_flag
   Returns the sky flags for the sky selected by the user.  */

   /* Sanity check */
   if(c == NULL) return(0);

   /* Lookup the sky flags */
   switch(c->land->realsky) {
      case SC_SKY_NIGHT:
         return(SC_LAND_NIGHT_SKY);
      case SC_SKY_FIRE:
         return(SC_LAND_FIRE_SKY);
      case SC_SKY_RANDOM:
         return(0);
   } /* End switch */

   return(0);

}
