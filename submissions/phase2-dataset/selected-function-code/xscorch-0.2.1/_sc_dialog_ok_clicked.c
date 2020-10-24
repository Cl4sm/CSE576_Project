static void _sc_dialog_ok_clicked(GtkWidget *button, ScDialog *dlg) {
/* sc_dialog_ok_clicked
   The OK button was clicked.  Emit the "apply" signal, then close
   out the dialog window (unless defer was selected).  */

   assert(IS_SC_DIALOG(dlg));

   /* Emit "apply" signal */
   _sc_dialog_apply_clicked(button, dlg);

   /* Update the current dialog state */
   if(dlg->state != NULL) {
      *dlg->state = SC_DIALOG_ACCEPTED;
   }
   
   /* Destroy the dialog window, unless the flags indicate we should defer. */
   if(!(dlg->flags & SC_DIALOG_DELAY_DESTROY)) {
      gtk_widget_destroy(GTK_WIDGET(dlg));
   }

}
