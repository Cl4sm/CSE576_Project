gboolean sc_display_console_detach(ScDisplay *dpy) {

   GtkFixedChild *child;
   GtkWidget *cwidget;
   GList *children;

   g_return_val_if_fail(IS_SC_DISPLAY(dpy), FALSE);

   children = GTK_FIXED(dpy)->children;

   /* The first entry is the basic drawbuf; never delete it.  */
   if(children == NULL || children->next == NULL) return(FALSE);
   while(children->next != NULL) children = children->next;
   child = children->data;
   cwidget = child->widget;
   g_return_val_if_fail(IS_SC_CONSOLE(cwidget), FALSE);

   gtk_container_remove(GTK_CONTAINER(dpy), cwidget);
   /* gtk_container_remove destroys the object on its own,
      apparently.  We do not need to call gtk_widget_destroy.  */
   return(TRUE);

}
