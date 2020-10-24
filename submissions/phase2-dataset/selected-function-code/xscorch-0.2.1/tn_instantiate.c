/*
 * tn_initiate
 * Create a new connection struct.
 * Will overwrite *tnc so don't pass it something precious!
 */

   /* Only operate on an extant pointer. */
   if(tnc == NULL) return(false);

   /* Allocate the new connection struct. */
   (*tnc) = tn_connection_new();
   if((*tnc) == NULL) return(false);

   (*tnc)->socket = socket;
   tn_set_state((*tnc), TN_STATE_CONNECTED);

   return(true);

}
