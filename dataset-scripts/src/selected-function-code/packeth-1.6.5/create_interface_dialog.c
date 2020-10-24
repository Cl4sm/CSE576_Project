GtkWidget*
create_interface_dialog (void)
{
  GtkWidget *interface_dialog;
  GtkWidget *dialog_vbox2;
  GtkWidget *hbox66;
  GtkWidget *label161;
  GtkWidget *combo1;
  GList *combo1_items = NULL;
  GtkWidget *combo_entry1;
  GtkWidget *dialog_action_area2;
  GtkWidget *hbox65;
  GtkWidget *button50;
  GtkWidget *button51;

  interface_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (interface_dialog), _("Options"));
  gtk_window_set_type_hint (GTK_WINDOW (interface_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox2 = GTK_DIALOG (interface_dialog)->vbox;
  gtk_widget_show (dialog_vbox2);

  hbox66 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox66);
  gtk_box_pack_start (GTK_BOX (dialog_vbox2), hbox66, FALSE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox66), 15);

  label161 = gtk_label_new (_("Select outgoing interface "));
  gtk_widget_show (label161);
  gtk_box_pack_start (GTK_BOX (hbox66), label161, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label161), GTK_JUSTIFY_CENTER);

  combo1 = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (combo1)->popwin),
                     "GladeParentKey", combo1);
  gtk_widget_show (combo1);
  gtk_box_pack_start (GTK_BOX (hbox66), combo1, TRUE, TRUE, 0);
  combo1_items = g_list_append (combo1_items, (gpointer) "");
  gtk_combo_set_popdown_strings (GTK_COMBO (combo1), combo1_items);
  g_list_free (combo1_items);

  combo_entry1 = GTK_COMBO (combo1)->entry;
  gtk_widget_show (combo_entry1);
  gtk_entry_set_invisible_char (GTK_ENTRY (combo_entry1), 8226);

  dialog_action_area2 = GTK_DIALOG (interface_dialog)->action_area;
  gtk_widget_show (dialog_action_area2);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area2), GTK_BUTTONBOX_END);

  hbox65 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox65);
  gtk_container_add (GTK_CONTAINER (dialog_action_area2), hbox65);

  button50 = gtk_button_new_with_mnemonic (_("OK"));
  gtk_widget_show (button50);
  gtk_box_pack_start (GTK_BOX (hbox65), button50, TRUE, FALSE, 0);

  button51 = gtk_button_new_with_mnemonic (_("Cancel"));
  gtk_widget_show (button51);
  gtk_box_pack_start (GTK_BOX (hbox65), button51, TRUE, FALSE, 0);

  g_signal_connect ((gpointer) interface_dialog, "destroy",
                    G_CALLBACK (on_interface_dialog_destroy),
                    NULL);
  g_signal_connect ((gpointer) button50, "clicked",
                    G_CALLBACK (on_button50_clicked),
                    NULL);
  g_signal_connect ((gpointer) button51, "clicked",
                    G_CALLBACK (on_button51_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (interface_dialog, interface_dialog, "interface_dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (interface_dialog, dialog_vbox2, "dialog_vbox2");
  GLADE_HOOKUP_OBJECT (interface_dialog, hbox66, "hbox66");
  GLADE_HOOKUP_OBJECT (interface_dialog, label161, "label161");
  GLADE_HOOKUP_OBJECT (interface_dialog, combo1, "combo1");
  GLADE_HOOKUP_OBJECT (interface_dialog, combo_entry1, "combo_entry1");
  GLADE_HOOKUP_OBJECT_NO_REF (interface_dialog, dialog_action_area2, "dialog_action_area2");
  GLADE_HOOKUP_OBJECT (interface_dialog, hbox65, "hbox65");
  GLADE_HOOKUP_OBJECT (interface_dialog, button50, "button50");
  GLADE_HOOKUP_OBJECT (interface_dialog, button51, "button51");

  return interface_dialog;
}