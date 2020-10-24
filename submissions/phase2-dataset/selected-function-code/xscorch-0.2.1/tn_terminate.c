bool tn_terminate(tn_connection **tnc) {
/*
 * tn_terminate
 * Close out a connection and free up its structs.
 */

   if(tnc == NULL || (*tnc) == NULL) return(true);

   /* Try and make sure the packet queues are flushed. */
   if(!TN_STATE_IS_ERROR(*tnc)) {
      tn_write_flush(*tnc);
      tn_scan_read(*tnc, NULL);
   }

   /* Shut the socket down. */
   if((*tnc)->state & TN_STATE_CONNECTED && (*tnc)->socket >= 0) {
      shutdown((*tnc)->socket, 2);
      close((*tnc)->socket);
      (*tnc)->socket = -1;
   }

   /* Free the structs. */
   tn_connection_free(tnc);

   return(true);

}
