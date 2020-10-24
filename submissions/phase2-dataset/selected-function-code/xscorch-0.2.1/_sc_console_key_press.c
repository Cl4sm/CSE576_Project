static gint _sc_console_key_press(GtkWidget *widget, GdkEventKey *event) {
/* sc_console_key_press
   Process a key that was pressed.  */

   ScConsole *cons = SC_CONSOLE(widget);

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->key_press_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->key_press_event(widget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   switch(event->keyval) {
      case GDK_Page_Up:
      case GDK_KP_Page_Up:
         if(_sc_console_can_scroll_up(cons)) {
            cons->text.viewy -= cons->text.viewh;
            if(cons->text.viewy < 0) cons->text.viewy = 0;
            _sc_console_draw_vert_scroll(cons);
            _sc_console_draw_region(cons, 0, 0, sc_console_get_width(cons), sc_console_get_height(cons));
         }
         return(TRUE);

      case GDK_Page_Down:
      case GDK_KP_Page_Down:
         if(_sc_console_can_scroll_down(cons)) {
            cons->text.viewy += cons->text.viewh;
            if(cons->text.viewy > sc_console_get_height(cons) - cons->text.viewh) {
               cons->text.viewy = sc_console_get_height(cons) - cons->text.viewh;
            }
            _sc_console_draw_vert_scroll(cons);
            _sc_console_draw_region(cons, 0, 0, sc_console_get_width(cons), sc_console_get_height(cons));
         }
         return(TRUE);
   } /* Search for special keys */

   /* Fallthrough */
   return(FALSE);

}
