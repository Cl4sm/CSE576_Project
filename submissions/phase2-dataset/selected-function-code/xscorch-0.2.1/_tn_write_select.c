bool _tn_write_select(tn_connection *tnc) {
/*
 * tn_write_select
 * Scan the connection buffer to see if there are errors.
 * Returns true if there are no errors (it's safe to try and send).
 */

   fd_set fds;
   struct timeval maxwait;

   /* Perpare the descriptor. */
   FD_ZERO(&fds);
   FD_SET(tnc->socket, &fds);

   /* Set the wait time to 0. */
   maxwait.tv_sec  = 0;
   maxwait.tv_usec = 0;

   /* Find out if there are errors. */
   switch(select(tnc->socket + 1, NULL, &fds, NULL, &maxwait)) {
      case  1:
      case  0:
         /* There are no errors. */
         return(true);
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
