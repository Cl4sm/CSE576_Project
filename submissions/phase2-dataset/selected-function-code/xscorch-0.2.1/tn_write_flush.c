/*
 * tn_write_flush
 * Flush the transmit buffer.
 * Returns true if the buffer is now empty.
 */

   tn_packet_list *pl;
   byte header[TN_HEADER_SIZE];

   /* Make sure the connection is up. */
   if(tnc == NULL || !TN_CONNECTION_IS_UP(tnc)) return(false);

   /* Check for socket errors. */
   if(!_tn_write_select(tnc)) return(false);

   /* Get the top unwritten packet in the write chain. */
   pl = tnc->outgoing;
   while(pl != NULL && pl->complete) pl = pl->next;

   /* Send any unsent packets on the queue. */
   while(pl != NULL) {
      /* Send the header. */
      ((dword *)header)[0] = htonl(pl->packet->bop);
      ((dword *)header)[1] = htonl(pl->packet->size);
      ((dword *)header)[2] = htonl(pl->packet->id);
      if(!tn_send_buffer(tnc, header, TN_HEADER_SIZE))
         return(false);

      /* Send the payload. */
      if(!tn_send_buffer(tnc, pl->packet->payload, pl->packet->size))
         return(false);

      /* Advance the packet pointer. */
      pl->complete = true;
      pl = pl->next;
   }

   return(true);

}
