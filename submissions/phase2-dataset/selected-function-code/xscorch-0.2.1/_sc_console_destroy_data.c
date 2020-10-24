static void _sc_console_destroy_data(ScConsole *cons) {
/* sc_console_destroy_data
   Destroys the data buffer currently allocated to this console, and
   data allocated in the highlight structures.  This does not release
   interface widgets.  */

   GList *cur;

   assert(IS_SC_CONSOLE(cons));

   /* Delete the console text buffer */
   if(_sc_console_get_text_buffer(cons) != NULL) {
      free(_sc_console_get_text_buffer(cons));
      cons->text.buffer = NULL;
   }

   /* Delete the highlights list */
   cur = cons->highlights;
   while(cur != NULL) {
      if(cur->data != NULL) free(cur->data);
      cur->data = NULL;
      cur = cur->next;
   }
   g_list_free(cons->highlights);
   cons->highlights = NULL;

}
