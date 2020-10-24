static void _sc_active_console_destroy(GtkObject *obj) {
/* sc_active_console_destroy
   Destroys the active console passed in.  */

   ScActiveConsole *cons = SC_ACTIVE_CONSOLE(obj);
   GList *cur = cons->spots;

   while(cur != NULL) {
      if(cur->data != NULL) {
         free(cur->data);
      }
      cur->data = NULL;
      cur = cur->next;
   }
   g_list_free(cons->spots);
   cons->spots = NULL;

   /* Call parent handler? */
   if(GTK_OBJECT_CLASS(parent_class)->destroy != NULL) {
      GTK_OBJECT_CLASS(parent_class)->destroy(obj);
   } /* Parent handler */

}
