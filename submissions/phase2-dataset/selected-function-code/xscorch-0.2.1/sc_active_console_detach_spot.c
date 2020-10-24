
   ScActiveConsoleSpot *spot;
   GList *cur;
   gint x;
   gint y;
   gint width;
   gint height;

   /* Is there anything to delete? */
   cur = cons->spots;
   if(cur == NULL) return(FALSE);

   /* reset the current selection */
   cons->current = NULL;
   _sc_active_console_leave_spot(cons, FALSE);
   sc_console_set_cursor(SC_CONSOLE(cons), 0, 0, 0, 0);
   sc_console_set_cursor_highlighted(SC_CONSOLE(cons), FALSE);

   /* Delete the last spot */
   while(cur->next != NULL) cur = cur->next;
   spot = cur->data;
   cur->data = NULL;

   /* Release any associated gadget */
   if(spot->gadget != NULL) {
      g_object_unref(GTK_OBJECT(spot->gadget));
      spot->gadget = NULL;
   }

   x = spot->x;
   y = spot->y;
   width = spot->width;
   height= spot->height;

   free(cur->data);
   cur->data = NULL;
   cons->spots = g_list_remove_link(cons->spots, cur);
   g_list_free(cur);

   return(TRUE);

}
