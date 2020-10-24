/* sc_console_key_release
   Process a key that was released.  */

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->key_release_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->key_release_event(widget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   switch(event->keyval) {
      case GDK_Page_Up:
      case GDK_KP_Page_Up:
      case GDK_Page_Down:
      case GDK_KP_Page_Down:
         return(TRUE);
   } /* Search for special keys */

   /* Fallthrough */
   return(FALSE);

}
