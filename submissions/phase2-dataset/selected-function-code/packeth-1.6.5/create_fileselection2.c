GtkWidget*
create_fileselection2 (void)
{
  GtkWidget *fileselection2;
  GtkWidget *dialog_vbox9;
  GtkWidget *dialog_action_area9;
  GtkWidget *cancel_button2;
  GtkWidget *ok_button2;

  fileselection2 = gtk_file_chooser_dialog_new (_("Save Dialog"), NULL, GTK_FILE_CHOOSER_ACTION_SAVE, NULL, 0, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (fileselection2), 1);
  gtk_window_set_type_hint (GTK_WINDOW (fileselection2), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox9 = GTK_DIALOG (fileselection2)->vbox;
  gtk_widget_show (dialog_vbox9);

  dialog_action_area9 = GTK_DIALOG (fileselection2)->action_area;
  gtk_widget_show (dialog_action_area9);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area9), GTK_BUTTONBOX_END);

  cancel_button2 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancel_button2);
  gtk_dialog_add_action_widget (GTK_DIALOG (fileselection2), cancel_button2, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancel_button2, GTK_CAN_DEFAULT);

  ok_button2 = gtk_button_new_from_stock ("gtk-save");
  gtk_widget_show (ok_button2);
  gtk_dialog_add_action_widget (GTK_DIALOG (fileselection2), ok_button2, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (ok_button2, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) fileselection2, "destroy",
                    G_CALLBACK (on_fileselection2_destroy),
                    NULL);
  g_signal_connect ((gpointer) cancel_button2, "clicked",
                    G_CALLBACK (on_cancel_button2_clicked),
                    NULL);
  g_signal_connect ((gpointer) ok_button2, "clicked",
                    G_CALLBACK (on_ok_button2_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection2, fileselection2, "fileselection2");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection2, dialog_vbox9, "dialog_vbox9");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection2, dialog_action_area9, "dialog_action_area9");
  GLADE_HOOKUP_OBJECT (fileselection2, cancel_button2, "cancel_button2");
  GLADE_HOOKUP_OBJECT (fileselection2, ok_button2, "ok_button2");

  gtk_widget_grab_default (ok_button2);
  return fileselection2;
}