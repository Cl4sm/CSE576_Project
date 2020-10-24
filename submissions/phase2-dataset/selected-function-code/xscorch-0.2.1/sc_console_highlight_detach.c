/* sc_console_highlight_detach
   Remove the most recently attached highlight on this console.  
   Returns FALSE if there were no consoles available for deletion.  */

   ScConsoleHighlight *high;  /* Highlight to delete */
   GList *cur;                /* Linked list iterator */
   gint x;                    /* X position of highlight */
   gint y;                    /* Y position of highlight */
   gint width;                /* Width of highlight */
   gint height;               /* Height of highlight */
   
   g_return_val_if_fail(IS_SC_CONSOLE(cons), FALSE);

   /* Is there anything to delete? */
   cur = cons->highlights;
   if(cur == NULL) return(FALSE);

   /* Delete the last highlight */
   while(cur->next != NULL) cur = cur->next;
   high = cur->data;
   g_return_val_if_fail(high != NULL, FALSE);
   cur->data = NULL;

   /* Find out the extents of the highlight */
   x = high->x;
   y = high->y;
   width = high->width;
   height= high->height;

   /* Release the highlight */
   free(high);
   cons->highlights = g_list_remove_link(cons->highlights, cur);
   g_list_free(cur);

   /* Update display */
   _sc_console_draw_region(cons, x, y, width, height);
   return(TRUE);

}
