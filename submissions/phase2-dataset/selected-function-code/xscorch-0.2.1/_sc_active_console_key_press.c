static gint _sc_active_console_key_press(GtkWidget *widget, GdkEventKey *event) {

   ScActiveConsole *cons = SC_ACTIVE_CONSOLE(widget);
   ScActiveConsoleSpot *spot;

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->key_press_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->key_press_event(widget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   if(cons->current == NULL) return(FALSE);
   if(!cons->allowkeyboard) return(FALSE);

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("key is %d (%s)   xx", event->keyval, gdk_keyval_name(event->keyval));
   #endif /* debug */

   switch(event->keyval) {
      case GDK_Up:
      case GDK_KP_Up:
         _sc_active_console_leave_spot(cons, FALSE);
         cons->current = cons->current->prev;
         if(cons->current == NULL) {
            cons->current = g_list_last(cons->spots);
         }
         spot = cons->current->data;
         _sc_active_console_enter_spot(cons, FALSE);
         sc_console_set_cursor(SC_CONSOLE(cons), spot->x, spot->y, spot->width, spot->height);
         return(TRUE);

      case GDK_Down:
      case GDK_KP_Down:
         _sc_active_console_leave_spot(cons, FALSE);
         cons->current = cons->current->next;
         if(cons->current == NULL) {
            cons->current = cons->spots;
         }
         spot = cons->current->data;
         _sc_active_console_enter_spot(cons, FALSE);
         sc_console_set_cursor(SC_CONSOLE(cons), spot->x, spot->y, spot->width, spot->height);
         return(TRUE);

      /* Note: there should be NO logic here for processing GDK_Return or
         GDK_KP_Enter.  Every dialogue seems to work fine without Return
         logic here, and the tank dialogue gets really messed up if we have
         processing for Return here.  */

      case GDK_space:
      case GDK_KP_Space:
         #if SC_GTK_DEBUG_GTK && __debugging_macros
            SC_DEBUG_ENTER("emitting %d", _sc_active_console_signals[SELECT_SPOT_SIGNAL]);
         #endif /* debug */
         spot = cons->current->data;
         if(_sc_active_console_emit(cons, SELECT_SPOT_SIGNAL, spot)) {
            return(TRUE);
         }
         if(_sc_active_console_emit_event(cons, KEY_PRESS_SPOT_SIGNAL, event, spot)) {
            return(TRUE);
         }
         break;

      default:
         #if SC_GTK_DEBUG_GTK && __debugging_macros
            SC_DEBUG_ENTER("emitting %d", _sc_active_console_signals[KEY_PRESS_SPOT_SIGNAL]);
         #endif /* debug */
         spot = cons->current->data;
         if(_sc_active_console_emit_event(cons, KEY_PRESS_SPOT_SIGNAL, event, spot)) {
            return(TRUE);
         }
         break;
   }

   return(FALSE);

}
