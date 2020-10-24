/* sc_player_activate_shield
   Activates the currently selected player shield.  If unable to activate,
   then false is returned.  Any existing shield will be destroyed, even
   if fully powered; so use with caution.  The player shield will also
   be advanced if necessary.  */

   sc_accessory_info *info;

   /* Sanity check */
   if(c == NULL || p == NULL) return(false);
   
   /* Verify this is a shield */
   info = p->selshield;
   if(!SC_ACCESSORY_IS_SHIELD(info) || info->inventories[p->index] <= 0) return(false);

   /* Activate this shield, perhaps destroying any old shielding */
   sc_shield_free(&p->shield);
   p->shield = sc_shield_new(info);

   /* Update state */
   #if USE_NETWORK
      sc_net_client_send_shields(c, c->client, p->index);
   #endif

   /* Update inventory - Must happen AFTER send_shields! */
   --info->inventories[p->index];
   sc_player_advance_shield(c, p, SC_PLAYER_SHIELD_CHECK_CUR);

   sc_status_update(c->window, p);
   return(true);

}
