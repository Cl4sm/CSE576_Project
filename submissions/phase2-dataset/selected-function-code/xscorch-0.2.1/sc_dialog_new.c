/* sc_dialog_new
   Create a new dialog window, with the indicated title, message text, and
   flags.  If title or msgtext are NULL, then they are omitted from the
   resultant dialog window.  */

   ScDialog *dialog;
   GtkWidget *vbox;
   GtkWidget *hbox;
   GtkWidget *msg;
   GtkWidget *btn;

   dialog = g_object_new(sc_dialog_get_type(), NULL);
   g_return_val_if_fail(dialog != NULL, NULL);

   if(title != NULL) {
      gtk_window_set_title(GTK_WINDOW(dialog), title);
   }

   vbox = gtk_vbox_new(FALSE, 5);
   gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
   gtk_container_add(GTK_CONTAINER(dialog), vbox);

   if(msgtext != NULL) {
      msg = gtk_label_new(msgtext);
      gtk_widget_set_size_request(msg, 350, -1);
      gtk_label_set_line_wrap(GTK_LABEL(msg), TRUE);
      gtk_box_pack_start(GTK_BOX(vbox), msg, TRUE, TRUE, 0);
   }
   if(flags & SC_DIALOG_NO_GRID) {
      dialog->grid = NULL;
   } else {
      dialog->grid = gtk_table_new(1, 1, FALSE);
      gtk_box_pack_start(GTK_BOX(vbox), dialog->grid, TRUE, TRUE, 0);
   }

   gtk_box_pack_start(GTK_BOX(vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
   hbox = gtk_hbox_new(FALSE, 5);
   /* This box houses the command buttons and should not rescale vertically */
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   if(flags & SC_DIALOG_CLOSE) {
      btn = gtk_button_new_with_label(" Close ");
      g_signal_connect(G_OBJECT(btn), "clicked", (GCallback)_sc_dialog_cancel_clicked, dialog);
      gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 5);
   }
   if(flags & SC_DIALOG_APPLY) {
      btn = gtk_button_new_with_label(" Apply ");
      g_signal_connect(G_OBJECT(btn), "clicked", (GCallback)_sc_dialog_apply_clicked, dialog);
      gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 5);
   }
   if(flags & SC_DIALOG_NO) {
      btn = gtk_button_new_with_label(" No ");
      g_signal_connect(G_OBJECT(btn), "clicked", (GCallback)_sc_dialog_cancel_clicked, dialog);
      gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 5);
   }
   if(flags & SC_DIALOG_CANCEL) {
      btn = gtk_button_new_with_label(" Cancel ");
      g_signal_connect(G_OBJECT(btn), "clicked", (GCallback)_sc_dialog_cancel_clicked, dialog);
      gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 5);
   }
   if(flags & SC_DIALOG_YES) {
      btn = gtk_button_new_with_label(" Yes ");
      g_signal_connect(G_OBJECT(btn), "clicked", (GCallback)_sc_dialog_ok_clicked, dialog);
      gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 5);
   }
   if(flags & SC_DIALOG_OK) {
      btn = gtk_button_new_with_label(" Ok ");
      g_signal_connect(G_OBJECT(btn), "clicked", (GCallback)_sc_dialog_ok_clicked, dialog);
      gtk_box_pack_end(GTK_BOX(hbox), btn, FALSE, FALSE, 5);
   }

   dialog->flags = flags;
   return(GTK_WIDGET(dialog));

}
