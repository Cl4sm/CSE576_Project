bool _tn_read_select(tn_connection *tnc, struct timeval *timeout) {
/*
 * tn_read_select
 * Scan the connection buffer to see if data is waiting.
 * Returns true if there is any data waiting to be read.
 * Waits for timeout if one is given.
 */

   fd_set fds;
   struct timeval maxwait;

   /* Perpare the descriptor. */
   FD_ZERO(&fds);
   FD_SET(tnc->socket, &fds);

   /* Set the maximum wait time. */
   maxwait.tv_sec  = timeout ? timeout->tv_sec  : 0;
   maxwait.tv_usec = timeout ? timeout->tv_usec : 0;

   /* Find out if there is data to read. */
   switch(select(tnc->socket + 1, &fds, NULL, NULL, &maxwait)) {
      case  1:
         /* Data is available to read. */
         return(true);
      case  0:
         /* There was no data to read. */
         return(false);
      case -1:
         /* There was an error. */
         tn_set_state(tnc, TN_STATE_ERR_SELECT);
         return(false);
      default:
         /* Wrong number of selectors modified ... uhh, lol. */
         tn_set_state(tnc, TN_STATE_ERR_UNKNOWN);
         return(false);
   }

}
