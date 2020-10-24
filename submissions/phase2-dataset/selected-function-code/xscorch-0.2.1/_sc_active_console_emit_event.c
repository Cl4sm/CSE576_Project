                                                     gpointer event, ScActiveConsoleSpot *spot) {
/* sc_active_console_emit_event
   Emits the indicated signalid, which should be KEY_PRESS_SPOT_SIGNAL,
   KEY_RELEASE_SPOT_SIGNAL, BUTTON_PRESS_SPOT_SIGNAL, or
   BUTTON_RELEASE_SPOT_SIGNAL, to the spot's gadget (if one is installed),
   and then to the console itself.  If either of these signals returns
   TRUE, indicating the signal has been handled, then this function will
   return TRUE.  This function returns FALSE if both emitted signals
   return FALSE, to indicate the signal may not be fully processed.  */

   gboolean rtnval = FALSE;
   const char *signalname = NULL;

   assert(IS_SC_ACTIVE_CONSOLE(cons));
   assert(event != NULL);
   assert(spot != NULL);
   
   switch(signalid) {
   case KEY_PRESS_SPOT_SIGNAL:
      signalname = "key_press_spot";
      break;
   case KEY_RELEASE_SPOT_SIGNAL:
      signalname = "key_release_spot";
      break;
   case BUTTON_PRESS_SPOT_SIGNAL:
      signalname = "button_press_spot";
      break;
   case BUTTON_RELEASE_SPOT_SIGNAL:
      signalname = "button_release_spot";
      break;
   default:
      printf("ERROR: received bad signal ID in sactiveconsole.\n");
      abort();
   }

   /* Check to see if the spot includes a gadget. */
   if(spot->gadget != NULL) {
      assert(IS_SC_GADGET(spot->gadget));
      g_signal_emit_by_name(GTK_OBJECT(spot->gadget), signalname, event, &rtnval);
      if(rtnval) {
         return(TRUE);
      }
   }
   
   /* Emit the select-spot signal to the console itself */
   g_signal_emit(GTK_OBJECT(cons), _sc_active_console_signals[signalid], 0, 
                 spot, event, &rtnval);
   if(rtnval) {
      return(TRUE);
   }
   
   /* The signal may require additional processing if we made it here */
   return(FALSE);

}
