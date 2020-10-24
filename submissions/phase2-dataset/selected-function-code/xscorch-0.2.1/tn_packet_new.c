/*
 * tn_packet_new
 * Create and default a new packet.
 */

   tn_packet *packet;

   /* Return NULL if we can't alloc the thing. */
   packet = (tn_packet *)malloc(sizeof(tn_packet));
   if(packet == NULL) return(NULL);

   /* Allocate the default buffer chunk. */
   packet->payload = (byte *)malloc(TN_BUFFER_SIZE * sizeof(byte));
   if(packet->payload == NULL) {
      free(packet);
      return(NULL);
   }
   memset(packet->payload, 0, TN_BUFFER_SIZE);

   /* Set defaults. */
   packet->bop     = TN_ALIGN_MUGGLE;
   packet->size    = 0;
   packet->id      = 0;

   return(packet);

}
