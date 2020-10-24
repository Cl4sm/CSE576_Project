static gint _sc_active_console_button_press(GtkWidget *widget, GdkEventButton *event) {

   ScActiveConsole *cons = SC_ACTIVE_CONSOLE(widget);
   ScActiveConsoleSpot *spot;
   gint currow;
   gint curcol;
   GList *cur;

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->button_press_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->button_press_event(widget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   /* Make sure this is a SINGLE click event */
   if(event->type != GDK_BUTTON_PRESS) return(FALSE);

   gtk_widget_grab_focus(GTK_WIDGET(cons));
   if(cons->current == NULL) return(FALSE);

   /* Make sure the click is within the usable window space */
   if(!_sc_active_console_adjust_xy(cons, event, &currow, &curcol)) return(FALSE);

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("will process a button press at row %d, col %d", currow, curcol);
   #endif /* debug */

   cur = cons->spots;
   while(cur != NULL) {
      spot = cur->data;
      if(curcol >= spot->x && curcol < spot->x + spot->width &&
       currow >= spot->y && currow < spot->y + spot->height) {
         _sc_active_console_leave_spot(cons, FALSE);
         cons->current = cur;
         _sc_active_console_enter_spot(cons, FALSE);
         sc_console_set_cursor(SC_CONSOLE(cons), spot->x, spot->y, spot->width, spot->height);
         sc_console_set_cursor_highlighted(SC_CONSOLE(cons), TRUE);
         _sc_active_console_emit_event(cons, BUTTON_PRESS_SPOT_SIGNAL, event, spot);
         return(TRUE);
      }
      cur = cur->next;
   }

   return(FALSE);

}
