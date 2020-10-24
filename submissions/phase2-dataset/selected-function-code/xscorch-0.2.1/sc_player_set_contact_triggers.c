/* sc_player_set_contact_triggers
   Toggles the contact-trigger flag automagically.
   Won't toggle it unless the player has contact triggers. */

   /* Sanity check */
   assert(c != NULL);
   assert(p != NULL);

   /* Update the flag */
   if(flag && (p->ac_state & SC_ACCESSORY_STATE_TRIGGER)) {
      p->contacttriggers = true;
   } else {
      p->contacttriggers = false;
   }

   /* Make sure player flags are transmitted over network */
   #if USE_NETWORK
      sc_net_client_send_flags(c, c->client, p->index);
   #endif

   /* Update status window. */
   sc_status_update(c->window, p);

}
