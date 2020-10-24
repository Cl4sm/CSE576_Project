GtkWidget*
create_fileselection3 (void)
{
  GtkWidget *fileselection3;
  GtkWidget *dialog_vbox10;
  GtkWidget *dialog_action_area10;
  GtkWidget *cancel_button3;
  GtkWidget *ok_button3;

  fileselection3 = gtk_file_chooser_dialog_new (_("Select Database"), NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL, 0, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (fileselection3), 5);
  gtk_window_set_type_hint (GTK_WINDOW (fileselection3), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox10 = GTK_DIALOG (fileselection3)->vbox;
  gtk_widget_show (dialog_vbox10);

  dialog_action_area10 = GTK_DIALOG (fileselection3)->action_area;
  gtk_widget_show (dialog_action_area10);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area10), GTK_BUTTONBOX_END);

  cancel_button3 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancel_button3);
  gtk_dialog_add_action_widget (GTK_DIALOG (fileselection3), cancel_button3, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancel_button3, GTK_CAN_DEFAULT);

  ok_button3 = gtk_button_new_from_stock ("gtk-open");
  gtk_widget_show (ok_button3);
  gtk_dialog_add_action_widget (GTK_DIALOG (fileselection3), ok_button3, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (ok_button3, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) fileselection3, "destroy",
                    G_CALLBACK (on_fileselection3_destroy),
                    NULL);
  g_signal_connect ((gpointer) cancel_button3, "clicked",
                    G_CALLBACK (on_cancel_button3_clicked),
                    NULL);
  g_signal_connect ((gpointer) ok_button3, "clicked",
                    G_CALLBACK (on_ok_button3_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection3, fileselection3, "fileselection3");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection3, dialog_vbox10, "dialog_vbox10");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection3, dialog_action_area10, "dialog_action_area10");
  GLADE_HOOKUP_OBJECT (fileselection3, cancel_button3, "cancel_button3");
  GLADE_HOOKUP_OBJECT (fileselection3, ok_button3, "ok_button3");

  gtk_widget_grab_default (ok_button3);
  return fileselection3;
}