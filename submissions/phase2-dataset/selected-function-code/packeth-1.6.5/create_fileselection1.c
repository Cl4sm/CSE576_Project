GtkWidget*
create_fileselection1 (void)
{
  GtkWidget *fileselection1;
  GtkWidget *dialog_vbox8;
  GtkWidget *dialog_action_area8;
  GtkWidget *cancel_button1;
  GtkWidget *ok_button1;

  fileselection1 = gtk_file_chooser_dialog_new (_("Load Dialog"), NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL, 0, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (fileselection1), 1);
  gtk_window_set_type_hint (GTK_WINDOW (fileselection1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox8 = GTK_DIALOG (fileselection1)->vbox;
  gtk_widget_show (dialog_vbox8);

  dialog_action_area8 = GTK_DIALOG (fileselection1)->action_area;
  gtk_widget_show (dialog_action_area8);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area8), GTK_BUTTONBOX_END);

  cancel_button1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancel_button1);
  gtk_dialog_add_action_widget (GTK_DIALOG (fileselection1), cancel_button1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancel_button1, GTK_CAN_DEFAULT);

  ok_button1 = gtk_button_new_from_stock ("gtk-open");
  gtk_widget_show (ok_button1);
  gtk_dialog_add_action_widget (GTK_DIALOG (fileselection1), ok_button1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (ok_button1, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) fileselection1, "destroy",
                    G_CALLBACK (on_fileselection1_destroy),
                    NULL);
  g_signal_connect ((gpointer) cancel_button1, "clicked",
                    G_CALLBACK (on_cancel_button1_clicked),
                    NULL);
  g_signal_connect ((gpointer) ok_button1, "clicked",
                    G_CALLBACK (on_ok_button1_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection1, fileselection1, "fileselection1");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection1, dialog_vbox8, "dialog_vbox8");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection1, dialog_action_area8, "dialog_action_area8");
  GLADE_HOOKUP_OBJECT (fileselection1, cancel_button1, "cancel_button1");
  GLADE_HOOKUP_OBJECT (fileselection1, ok_button1, "ok_button1");

  gtk_widget_grab_default (ok_button1);
  return fileselection1;
}