
   ScActiveConsole *cons = SC_ACTIVE_CONSOLE(widget);
   ScActiveConsoleSpot *spot;
   gint currow;
   gint curcol;
   GList *cur;

   /* Try out parent handler first */
   if(GTK_WIDGET_CLASS(parent_class)->button_release_event != NULL) {
      if(GTK_WIDGET_CLASS(parent_class)->button_release_event(widget, event)) {
         /* Crap. The signal's already been handled */
         return(TRUE);
      } /* Signal processed? */
   } /* Signal handler available? */

   gtk_widget_grab_focus(GTK_WIDGET(cons));
   if(cons->current == NULL) return(FALSE);

   /* Adjust click so it is within the usable window space */
   _sc_active_console_adjust_xy(cons, event, &currow, &curcol);

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("will process a button release at row %d, col %d", currow, curcol);
   #endif /* debug */

   spot = cons->current->data;
   if(cons->allowkeyboard) {
      /* Maintain the cursor, just unhighlight it */
      sc_console_set_cursor(SC_CONSOLE(cons), spot->x, spot->y, spot->width, spot->height);
      sc_console_set_cursor_highlighted(SC_CONSOLE(cons), FALSE);
   } else {
      /* No keyboard allowed; just destroy the cursor */
      sc_console_set_cursor(SC_CONSOLE(cons), 0, 0, 0, 0);
      sc_console_set_cursor_highlighted(SC_CONSOLE(cons), FALSE);
   }

   cur = cons->spots;
   while(cur != NULL) {
      spot = cur->data;
      if(curcol >= spot->x && curcol < spot->x + spot->width &&
         currow >= spot->y && currow < spot->y + spot->height) {
         if(cons->current == cur) {
            if(_sc_active_console_emit(cons, SELECT_SPOT_SIGNAL, spot)) {
               return(TRUE);
            }
         }
         _sc_active_console_emit_event(cons, BUTTON_RELEASE_SPOT_SIGNAL, event, spot);
         return(TRUE);
      }
      cur = cur->next;
   }

   return(FALSE);

}
