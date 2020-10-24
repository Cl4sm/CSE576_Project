/* sc_console_configure
   This is the configure event when a console is resized.  Takes
   care of redrawing the entire console.  */

   /* Check for a parent signal handler */
   if(GTK_WIDGET_CLASS(parent_class)->configure_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->configure_event(widget, event)) {
         /* Oops; we must halt */
         return(TRUE);
      } /* Can we continue? */
   } /* Does parent have default? */

   /* Draw the console */
   _sc_console_draw_all(SC_CONSOLE(widget));

   /* Let other events run as well... */
   return(FALSE);

}
