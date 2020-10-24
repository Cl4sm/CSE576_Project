/* sc_console_button_press
   Someone clicked the mouse.  Update the cursor/active highlight if
   needed, and check for scrollbar events.  */

   ScConsole *cons = SC_CONSOLE(widget);
   gboolean needredraw = FALSE;
   gint arrowh;
   gint height;
   gint width;
   gint size;
   gint pos;
   gint x;
   gint y;

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->button_press_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->button_press_event(widget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   /* Make sure this is a SINGLE click event */
   if(event->type != GDK_BUTTON_PRESS) return(FALSE);

   /* See if this was a click onto a scrollbar */
   if(cons->text.scrolly) {
      /* Check to see if we're clicking in vertical scrollbar region */
      _sc_console_vert_scroll_extents(cons, &x, &y, &width, &height, &arrowh);
      _sc_console_vert_trough_extents(cons, y, height, &pos, &size);
      if(event->x >= x && event->x < x + width &&
        event->y >= y - arrowh && event->y < y + height + arrowh) {
         /* Click was onto the scrollbar itself */
         if(event->y >= y - arrowh && event->y < y) {
            /* Clicked up-arrow */
            if(_sc_console_can_scroll_up(cons)) {
               --cons->text.viewy;
               needredraw = TRUE;
            } /* Can we scroll down? */
            needredraw = TRUE;
         } else if(event->y >= y + height && event->y < y + height + arrowh) {
            if(_sc_console_can_scroll_down(cons)) {
               ++cons->text.viewy;
               needredraw = TRUE;
            } /* Can we scroll down? */
         } else if(event->y < pos) {
            /* Page up */
            if(_sc_console_can_scroll_up(cons)) {
               cons->text.viewy -= cons->text.viewh;
               if(cons->text.viewy < 0) cons->text.viewy = 0;
               needredraw = TRUE;
            } /* Can we page up? */
         } else if(event->y >= pos + size) {
            /* Page down */
            if(_sc_console_can_scroll_down(cons)) {
               cons->text.viewy += cons->text.viewh;
               if(cons->text.viewy > sc_console_get_height(cons) - cons->text.viewh) {
                  cons->text.viewy = sc_console_get_height(cons) - cons->text.viewh;
               }
               needredraw = TRUE;
            } /* Can we page up? */
         } /* Checking Y coordinate */

         /* Redraw components? */
         if(needredraw) {
            _sc_console_draw_vert_scroll(cons);
            _sc_console_draw_region(cons, 0, 0, sc_console_get_width(cons), sc_console_get_height(cons));
         }

         /* Click was in scrollbar, abort */
         cons->ignorerelease = TRUE;
         return(TRUE);
      } /* Checking X, Y coordinate */
   } /* Checking vertical scrollbar */

   /* Nothing interesting... */
   return(FALSE);

}
