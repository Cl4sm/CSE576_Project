bool tn_write(tn_connection *tnc, const byte *payload, size_t size) {
/*
 * tn_write
 * Write a packet onto the outgoing chain.
 */

   int counter = 0;
   tn_packet_list *pl;

   if(tnc == NULL || payload == NULL || size <= 0) return(false);

   /* Tack the packet onto the chain. */
   if(tnc->outgoing == NULL) {
      /* Starting a new chain. */
      tnc->outgoing = tn_packet_list_new();
      pl = tnc->outgoing;
   } else {
      /* Tack onto a chain. */
      pl = tnc->outgoing;
      while(pl->next != NULL) {
         if(pl->complete) ++counter;
         pl = pl->next;
      }
      pl->next = tn_packet_list_new();
      if(pl->complete) ++counter;
      pl = pl->next;
   }

   /* Make sure the packet list allocation succeeded. */
   if(pl == NULL) {
      tn_set_state(tnc, TN_STATE_ERR_MALLOC);
      return(false);
   }

   /* Allocate the packet data payload. */
   pl->packet->payload = (byte *)malloc(size);
   if(pl->packet->payload == NULL) {
      tn_packet_list_free(&pl);
      tn_set_state(tnc, TN_STATE_ERR_MALLOC);
      return(false);
   }

   /* Set up the new outgoing packet. */
   memcpy(pl->packet->payload, payload, size);
   pl->packet->bop  = TN_ALIGN_MAGIC;
   pl->packet->id   = ++(tnc->out_id);
   pl->packet->size = size;

   /* Drop expired packets. */
   pl = tnc->outgoing;
   while(--counter >= TN_LEN_RETAIN_OLD) {
      tnc->outgoing = pl->next;
      tn_packet_list_free(&pl);
      pl = tnc->outgoing;
   }

   return(true);

}
