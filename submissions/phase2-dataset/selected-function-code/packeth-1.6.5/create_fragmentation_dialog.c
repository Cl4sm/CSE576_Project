GtkWidget*
create_fragmentation_dialog (void)
{
  GtkWidget *fragmentation_dialog;
  GtkWidget *dialog_vbox7;
  GtkWidget *frame44;
  GtkWidget *table6;
  GtkWidget *label301;
  GtkWidget *label302;
  GtkWidget *radiobutton55;
  GSList *radiobutton55_group = NULL;
  GtkWidget *radiobutton56;
  GtkWidget *radiobutton57;
  GSList *radiobutton57_group = NULL;
  GtkWidget *radiobutton58;
  GtkWidget *dialog_action_area7;
  GtkWidget *hbox119;
  GtkWidget *button79;
  GtkWidget *button80;

  fragmentation_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (fragmentation_dialog), _("Select fragmentation"));
  gtk_window_set_type_hint (GTK_WINDOW (fragmentation_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox7 = GTK_DIALOG (fragmentation_dialog)->vbox;
  gtk_widget_show (dialog_vbox7);

  frame44 = gtk_frame_new (NULL);
  gtk_widget_show (frame44);
  gtk_box_pack_start (GTK_BOX (dialog_vbox7), frame44, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame44), 10);

  table6 = gtk_table_new (2, 3, FALSE);
  gtk_widget_show (table6);
  gtk_container_add (GTK_CONTAINER (frame44), table6);
  gtk_container_set_border_width (GTK_CONTAINER (table6), 23);
  gtk_table_set_row_spacings (GTK_TABLE (table6), 5);
  gtk_table_set_col_spacings (GTK_TABLE (table6), 20);

  label301 = gtk_label_new (_("Don't fragment"));
  gtk_widget_show (label301);
  gtk_table_attach (GTK_TABLE (table6), label301, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label301), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label301), 0, 0.5);

  label302 = gtk_label_new (_("More fragments"));
  gtk_widget_show (label302);
  gtk_table_attach (GTK_TABLE (table6), label302, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label302), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label302), 0, 0.5);

  radiobutton55 = gtk_radio_button_new_with_mnemonic (NULL, _("Set"));
  gtk_widget_show (radiobutton55);
  gtk_table_attach (GTK_TABLE (table6), radiobutton55, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton55), radiobutton55_group);
  radiobutton55_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton55));

  radiobutton56 = gtk_radio_button_new_with_mnemonic (NULL, _("Not set"));
  gtk_widget_show (radiobutton56);
  gtk_table_attach (GTK_TABLE (table6), radiobutton56, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton56), radiobutton55_group);
  radiobutton55_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton56));

  radiobutton57 = gtk_radio_button_new_with_mnemonic (NULL, _("Set"));
  gtk_widget_show (radiobutton57);
  gtk_table_attach (GTK_TABLE (table6), radiobutton57, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton57), radiobutton57_group);
  radiobutton57_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton57));

  radiobutton58 = gtk_radio_button_new_with_mnemonic (NULL, _("Not set"));
  gtk_widget_show (radiobutton58);
  gtk_table_attach (GTK_TABLE (table6), radiobutton58, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton58), radiobutton57_group);
  radiobutton57_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton58));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton58), TRUE);

  dialog_action_area7 = GTK_DIALOG (fragmentation_dialog)->action_area;
  gtk_widget_show (dialog_action_area7);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area7), GTK_BUTTONBOX_END);

  hbox119 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox119);
  gtk_container_add (GTK_CONTAINER (dialog_action_area7), hbox119);

  button79 = gtk_button_new_with_mnemonic (_("OK"));
  gtk_widget_show (button79);
  gtk_box_pack_start (GTK_BOX (hbox119), button79, TRUE, FALSE, 0);

  button80 = gtk_button_new_with_mnemonic (_("Cancel"));
  gtk_widget_show (button80);
  gtk_box_pack_start (GTK_BOX (hbox119), button80, TRUE, FALSE, 0);

  g_signal_connect ((gpointer) fragmentation_dialog, "destroy",
                    G_CALLBACK (on_fragmentation_dialog_destroy),
                    NULL);
  g_signal_connect ((gpointer) button79, "clicked",
                    G_CALLBACK (on_button79_clicked),
                    NULL);
  g_signal_connect ((gpointer) button80, "clicked",
                    G_CALLBACK (on_button80_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (fragmentation_dialog, fragmentation_dialog, "fragmentation_dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (fragmentation_dialog, dialog_vbox7, "dialog_vbox7");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, frame44, "frame44");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, table6, "table6");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, label301, "label301");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, label302, "label302");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, radiobutton55, "radiobutton55");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, radiobutton56, "radiobutton56");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, radiobutton57, "radiobutton57");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, radiobutton58, "radiobutton58");
  GLADE_HOOKUP_OBJECT_NO_REF (fragmentation_dialog, dialog_action_area7, "dialog_action_area7");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, hbox119, "hbox119");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, button79, "button79");
  GLADE_HOOKUP_OBJECT (fragmentation_dialog, button80, "button80");

  return fragmentation_dialog;
}