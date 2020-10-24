/*
 * tn_send_buffer
 * Transmist a buffer with fully blocking IO.
 */

   int sent;

   while(size) {
      errno = 0;
      sent = send(tnc->socket, buffer, size, MSG_FLAGS);
      if(sent < 0) {
         switch(errno) {
            case EAGAIN:
               /* Wait a bit before we loop. */
               usleep(TN_BLOCKING_TIMEOUT);
            case EINTR:
               /* Loop around again and try once more. */
               break;
            case ENOBUFS:
            case ENOMEM:
               /* Set error to OOM. */
               tn_set_state(tnc, TN_STATE_ERR_MALLOC);
               return(false);
            case EPIPE:
               /* Connection state entered shutdown. */
               tn_set_state(tnc, TN_STATE_ERR_CONNLOST);
               return(false);
            case 0:
            default:
               /* Dunno what's wrong. */
               tn_set_state(tnc, TN_STATE_ERR_UNKNOWN);
               return(false);
         }
      } else {
         /* Partial or full transmission. */
         size -= sent;
         buffer += sent;
      }
   }

   return(true);

}
