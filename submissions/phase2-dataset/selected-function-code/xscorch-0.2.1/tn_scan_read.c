/*
 * tn_scan_read
 * Scan the connection buffer and read in new packets.
 * Returns true if there are any packets awaiting a read.
 * Waits for timeout if one is given.
 */

   bool ret;
   int length;
   tn_packet_list *pl;
   byte buffer[TN_BUFFER_SIZE];
   byte *bufptr;

   /* Make sure the connection is up. */
   if(tnc == NULL || !TN_CONNECTION_IS_UP(tnc)) return(false);

   /* Check for data. */
   if(!_tn_read_select(tnc, timeout)) return(false);

   /* Set up the working pointer. */
   pl = tnc->incoming;

   /* There may be no packets at all... */
   if(pl == NULL) {
      pl = tn_packet_list_new();
      if(pl == NULL) {
         tn_set_state(tnc, TN_STATE_ERR_MALLOC);
         return(false);
      }
      tnc->incoming = pl;
   }

   /* Find the last packet in the read chain. */
   while(pl->next != NULL) pl = pl->next;

   /* Read the data. */
   errno = 0;
   ret = false;
   while((length = recv(tnc->socket, buffer, TN_BUFFER_SIZE, MSG_FLAGS))) {

      /* Check for errors. */
      if(length < 0) {
         switch(errno) {
            case ENOTCONN:
            case ENOTSOCK:
               /* Connection is down. */
               tn_set_state(tnc, TN_STATE_ERR_CONNLOST);
               return(false);
            case EAGAIN:
               /* We've seen all there is to see here today. */
               return(ret);
            case EINTR:
               /* Wait and pick up the rest on the next call. */
               return(false);
            case 0:
            default:
               /* Unknown error, whee! */
               tn_set_state(tnc, TN_STATE_ERR_UNKNOWN);
               return(false);
         }
      }

      /* Iterate on new packets until we run out of data. */
      bufptr = buffer;
      while(length) {
         /* Allocate a new packet, if needed. */
         if(pl->complete) {
            pl->next = tn_packet_list_new();
            if(pl->next == NULL) {
               tn_set_state(tnc, TN_STATE_ERR_MALLOC);
               return(false);
            }
            pl = pl->next;
         }

         /* Complete the header. */
         if(tn_buffer_copy((pl->packet->payload + pl->size), &(bufptr),
                           &(pl->size), &(length), TN_HEADER_SIZE - pl->size)) {
            /* Perform tasks that happen once per packet. */
            if(pl->packet->bop == TN_ALIGN_MUGGLE) {
               /* Read the packet control data with proper byte ordering. */
               pl->packet->bop  = ntohl(((dword *)pl->packet->payload)[0]);
               pl->packet->size = ntohl(((dword *)pl->packet->payload)[1]);
               pl->packet->id   = ntohl(((dword *)pl->packet->payload)[2]);

               /* Security measure.  Avoid people spoofing incomplete packets. */
               if(pl->packet->bop == TN_ALIGN_MUGGLE) pl->packet->bop = 0;

               /* Allocate space for the payload if necessary. */
               if(pl->packet->size > TN_BUFFER_SIZE) {
                  pl->packet->payload = (byte *)realloc(pl->packet->payload, pl->packet->size * sizeof(byte));
                  if(pl->packet->payload == NULL) {
                     tn_set_state(tnc, TN_STATE_ERR_MALLOC);
                     return(false);
                  }
               }
            }
            /* Test for packet compliance via alignment. */
            if(pl->packet->bop != TN_ALIGN_MAGIC) {
               tn_set_state(tnc, TN_STATE_ERR_MISALIGN);
               return(false);
            }
            /* Test for packet compliance via ordering. */
            if(pl->packet->id != tnc->inc_id + 1) {
               tn_set_state(tnc, TN_STATE_ERR_MISORDER);
               return(false);
            }
            /* Complete the payload. */
            if(tn_buffer_copy((pl->packet->payload + pl->size - TN_HEADER_SIZE),
                              &(bufptr), &(pl->size), &(length),
                              pl->packet->size - pl->size + TN_HEADER_SIZE)) {
               pl->complete = true;
               ++(tnc->inc_id);
               ret = true;
            }
         }
      }
      errno = 0;
   }

   return(ret);

}
