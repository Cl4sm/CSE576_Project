/*
 * tn_connection_new
 * Create a new connection descriptor.
 */

   tn_connection *tnc;

   /* Try to snag some mem. */
   tnc = (tn_connection *)malloc(sizeof(tn_connection));
   if(tnc == NULL) return(NULL);

   /* Set up defaults. */
   tnc->state = TN_STATE_DEFAULT;
   tnc->socket = -1;
   tnc->inc_id = 0;
   tnc->out_id = 0;
   tnc->incoming = NULL;
   tnc->outgoing = NULL;

   return(tnc);

}
