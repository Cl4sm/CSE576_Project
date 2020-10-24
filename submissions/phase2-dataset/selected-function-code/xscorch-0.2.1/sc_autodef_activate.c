/* sc_player_activate_auto_def
   Activates player auto-defense, if available and if any shields are currently
   available.  This is for the interactive GUI screen (see ...auto_shield). */

   /* Sanity checks. */
   if(c == NULL || p == NULL || ads == NULL) return(false);

   /* Check for auto defense capability. */
   if(!(p->ac_state & SC_ACCESSORY_STATE_AUTO_DEF)) return(false);

   /*
    * Perform the requested auto defense actions...
    */

   /* Select the given tracking accessory. */
   /* TEMP - unimplemented - ads->auto_guidance - JL */

   /* Select and activate the given shield. */
   sc_player_set_shield(c, p, ads->auto_shield);
   sc_player_activate_shield(c, p);

   /* Activate parachutes, with the given pixel threshold. */
   /* TEMP - unimplemented - ads->chute_height - JL */

   /* Turn on or off Contact Triggers, as requested. */
   sc_player_set_contact_triggers(c, p, ads->triggers);

   /* Success! */
   return(true);

}
