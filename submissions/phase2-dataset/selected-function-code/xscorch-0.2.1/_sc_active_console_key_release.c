static gint _sc_active_console_key_release(GtkWidget *widget, GdkEventKey *event) {

   ScActiveConsole *cons = SC_ACTIVE_CONSOLE(widget);
   ScActiveConsoleSpot *spot;

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->key_release_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->key_release_event(widget, event)) {
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
      case GDK_Down:
      case GDK_KP_Down:
      case GDK_space:
      case GDK_KP_Space:
         #if SC_GTK_DEBUG_GTK && __debugging_macros
            SC_DEBUG_ENTER("ignored%s", "");
         #endif /* debug */
         return(TRUE);

      default:
         #if SC_GTK_DEBUG_GTK && __debugging_macros
            SC_DEBUG_ENTER("emitting %d", _sc_active_console_signals[KEY_RELEASE_SPOT_SIGNAL]);
         #endif /* debug */
         spot = cons->current->data;
         if(_sc_active_console_emit_event(cons, KEY_RELEASE_SPOT_SIGNAL, event, spot)) {
            return(TRUE);
         }
         break;
   }

   return(FALSE);

}
