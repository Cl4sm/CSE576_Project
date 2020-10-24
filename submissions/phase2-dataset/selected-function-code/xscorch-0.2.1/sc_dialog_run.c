gboolean sc_dialog_run(ScDialog *dialog) {
/* sc_dialog_run
   Runs the specified dialogue until it is closed.  This will set the
   dialog to modal if it was requested on creation, so other windows
   will be blocked while this dialog is visible.  */

   ScDialogState state;

   dialog->flags |= SC_DIALOG_DELAY_DESTROY;
   state = SC_DIALOG_WAITING;
   dialog->state = &state;

   gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_MOUSE);
   gtk_widget_show_all(GTK_WIDGET(dialog));
   if(!(dialog->flags & SC_DIALOG_NONMODAL)) {
      gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
   }

   while(state == SC_DIALOG_WAITING) {
      do {
         gtk_main_iteration();
      } while(gtk_events_pending());
   }

   dialog->state = NULL;
   if(state != SC_DIALOG_DESTROYED) {
      gtk_widget_destroy(GTK_WIDGET(dialog));
   }
   return(state == SC_DIALOG_ACCEPTED);

}
