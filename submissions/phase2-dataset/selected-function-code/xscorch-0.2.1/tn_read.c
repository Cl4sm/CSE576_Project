bool tn_read(tn_connection *tnc, byte **payload, size_t *size) {
/*
 * tn_read
 * Read a packet payload into a newly created buffer.
 * Upon returning, size will be the length of the payload.
 */

   tn_packet_list *pl;

   /* Basic requirements. */
   if(payload == NULL || size == NULL) return(false);

   /* If the packet exists and is a completed packet: */
   if(tnc != NULL && tnc->incoming != NULL && tnc->incoming->complete) {
      /* Drop it off the incoming packet chain. */
      pl = tnc->incoming;
      tnc->incoming = tnc->incoming->next;

      /* Extract the packet payload. */
      (*payload) = pl->packet->payload;
      (*size)    = pl->packet->size;

      /* Destroy the packet list item. */
      pl->packet->payload = NULL;
      tn_packet_list_free(&pl);
      return(true);
   } else {
      /* No packets were available. */
      (*payload) = NULL;
      (*size)    = 0;
      return(false);
   }

}
