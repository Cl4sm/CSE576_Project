static gint _sc_console_button_release(GtkWidget *widget, GdkEventButton *event) {
/* sc_console_button_release
   Someone released the mouse button.  Update the cursor/active highlight if
   needed, and check for scrollbar events.  */

   ScConsole *cons = SC_CONSOLE(widget);

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->button_release_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->button_release_event(widget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   /* See if this was a click onto a scrollbar */
   if(cons->ignorerelease) {
      cons->ignorerelease = FALSE;
      return(TRUE);
   } /* Clicked a scrollbar */

   /* Nothing interesting... */
   return(FALSE);

}
