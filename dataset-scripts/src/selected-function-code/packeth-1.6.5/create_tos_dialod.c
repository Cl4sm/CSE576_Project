GtkWidget*
create_tos_dialod (void)
{
  GtkWidget *tos_dialod;
  GtkWidget *dialog_vbox6;
  GtkWidget *vbox64;
  GtkWidget *hbox104;
  GtkWidget *radiobutton38;
  GSList *radiobutton38_group = NULL;
  GtkWidget *radiobutton39;
  GtkWidget *hbox105;
  GtkWidget *frame42;
  GtkWidget *vbox65;
  GtkWidget *hbox106;
  GtkWidget *label277;
  GtkWidget *optionmenu13;
  GtkWidget *convertwidget132;
  GtkWidget *convertwidget133;
  GtkWidget *convertwidget134;
  GtkWidget *convertwidget135;
  GtkWidget *convertwidget136;
  GtkWidget *convertwidget137;
  GtkWidget *convertwidget138;
  GtkWidget *convertwidget139;
  GtkWidget *convertwidget140;
  GtkWidget *table5;
  GtkWidget *label279;
  GtkWidget *label280;
  GtkWidget *label281;
  GtkWidget *label282;
  GtkWidget *radiobutton47;
  GSList *radiobutton47_group = NULL;
  GtkWidget *radiobutton48;
  GtkWidget *radiobutton49;
  GSList *radiobutton49_group = NULL;
  GtkWidget *radiobutton50;
  GtkWidget *radiobutton51;
  GSList *radiobutton51_group = NULL;
  GtkWidget *radiobutton52;
  GtkWidget *radiobutton53;
  GSList *radiobutton53_group = NULL;
  GtkWidget *radiobutton54;
  GtkWidget *frame43;
  GtkWidget *vbox66;
  GtkWidget *label278;
  GtkWidget *hbox111;
  GtkWidget *entry154;
  GtkWidget *dialog_action_area6;
  GtkWidget *hbox103;
  GtkWidget *button76;
  GtkWidget *button77;

  tos_dialod = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (tos_dialod), _("Select TOS value"));
  gtk_window_set_type_hint (GTK_WINDOW (tos_dialod), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox6 = GTK_DIALOG (tos_dialod)->vbox;
  gtk_widget_show (dialog_vbox6);

  vbox64 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox64);
  gtk_box_pack_start (GTK_BOX (dialog_vbox6), vbox64, TRUE, TRUE, 0);

  hbox104 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox104);
  gtk_box_pack_start (GTK_BOX (vbox64), hbox104, FALSE, FALSE, 15);

  radiobutton38 = gtk_radio_button_new_with_mnemonic (NULL, _("Type Of Service (TOS)"));
  gtk_widget_show (radiobutton38);
  gtk_box_pack_start (GTK_BOX (hbox104), radiobutton38, TRUE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton38), radiobutton38_group);
  radiobutton38_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton38));

  radiobutton39 = gtk_radio_button_new_with_mnemonic (NULL, _("Differentiated Services (DS)"));
  gtk_widget_show (radiobutton39);
  gtk_box_pack_start (GTK_BOX (hbox104), radiobutton39, TRUE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton39), radiobutton38_group);
  radiobutton38_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton39));

  hbox105 = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (hbox105);
  gtk_box_pack_start (GTK_BOX (vbox64), hbox105, TRUE, TRUE, 0);

  frame42 = gtk_frame_new (NULL);
  gtk_widget_show (frame42);
  gtk_box_pack_start (GTK_BOX (hbox105), frame42, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame42), 10);

  vbox65 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox65);
  gtk_container_add (GTK_CONTAINER (frame42), vbox65);
  gtk_container_set_border_width (GTK_CONTAINER (vbox65), 10);

  hbox106 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox106);
  gtk_box_pack_start (GTK_BOX (vbox65), hbox106, FALSE, FALSE, 0);

  label277 = gtk_label_new (_("Precedence "));
  gtk_widget_show (label277);
  gtk_box_pack_start (GTK_BOX (hbox106), label277, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label277), GTK_JUSTIFY_CENTER);

  optionmenu13 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu13);
  gtk_box_pack_start (GTK_BOX (hbox106), optionmenu13, FALSE, FALSE, 0);

  convertwidget132 = gtk_menu_new ();

  convertwidget133 = gtk_menu_item_new_with_mnemonic (_("000 - Routine"));
  gtk_widget_show (convertwidget133);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget133);

  convertwidget134 = gtk_menu_item_new_with_mnemonic (_("001 - Priority"));
  gtk_widget_show (convertwidget134);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget134);

  convertwidget135 = gtk_menu_item_new_with_mnemonic (_("010 - Immediate"));
  gtk_widget_show (convertwidget135);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget135);

  convertwidget136 = gtk_menu_item_new_with_mnemonic (_("011 - Flash"));
  gtk_widget_show (convertwidget136);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget136);

  convertwidget137 = gtk_menu_item_new_with_mnemonic (_("100 - Flash Overide"));
  gtk_widget_show (convertwidget137);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget137);

  convertwidget138 = gtk_menu_item_new_with_mnemonic (_("101 - Critic/Ecp"));
  gtk_widget_show (convertwidget138);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget138);

  convertwidget139 = gtk_menu_item_new_with_mnemonic (_("110 - Internetwork Control"));
  gtk_widget_show (convertwidget139);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget139);

  convertwidget140 = gtk_menu_item_new_with_mnemonic (_("111 - Network Control"));
  gtk_widget_show (convertwidget140);
  gtk_container_add (GTK_CONTAINER (convertwidget132), convertwidget140);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu13), convertwidget132);

  table5 = gtk_table_new (4, 3, FALSE);
  gtk_widget_show (table5);
  gtk_box_pack_start (GTK_BOX (vbox65), table5, TRUE, TRUE, 5);
  gtk_table_set_row_spacings (GTK_TABLE (table5), 3);
  gtk_table_set_col_spacings (GTK_TABLE (table5), 20);

  label279 = gtk_label_new (_("Delay"));
  gtk_widget_show (label279);
  gtk_table_attach (GTK_TABLE (table5), label279, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label279), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label279), 0, 0.5);

  label280 = gtk_label_new (_("Throughput"));
  gtk_widget_show (label280);
  gtk_table_attach (GTK_TABLE (table5), label280, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label280), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label280), 0, 0.5);

  label281 = gtk_label_new (_("Reliability"));
  gtk_widget_show (label281);
  gtk_table_attach (GTK_TABLE (table5), label281, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label281), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label281), 0, 0.5);

  label282 = gtk_label_new (_("Cost"));
  gtk_widget_show (label282);
  gtk_table_attach (GTK_TABLE (table5), label282, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label282), GTK_JUSTIFY_CENTER);
  gtk_misc_set_alignment (GTK_MISC (label282), 0, 0.5);

  radiobutton47 = gtk_radio_button_new_with_mnemonic (NULL, _("Normal"));
  gtk_widget_show (radiobutton47);
  gtk_table_attach (GTK_TABLE (table5), radiobutton47, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton47), radiobutton47_group);
  radiobutton47_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton47));

  radiobutton48 = gtk_radio_button_new_with_mnemonic (NULL, _("Low"));
  gtk_widget_show (radiobutton48);
  gtk_table_attach (GTK_TABLE (table5), radiobutton48, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton48), radiobutton47_group);
  radiobutton47_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton48));

  radiobutton49 = gtk_radio_button_new_with_mnemonic (NULL, _("Normal"));
  gtk_widget_show (radiobutton49);
  gtk_table_attach (GTK_TABLE (table5), radiobutton49, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton49), radiobutton49_group);
  radiobutton49_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton49));

  radiobutton50 = gtk_radio_button_new_with_mnemonic (NULL, _("High"));
  gtk_widget_show (radiobutton50);
  gtk_table_attach (GTK_TABLE (table5), radiobutton50, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton50), radiobutton49_group);
  radiobutton49_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton50));

  radiobutton51 = gtk_radio_button_new_with_mnemonic (NULL, _("Normal"));
  gtk_widget_show (radiobutton51);
  gtk_table_attach (GTK_TABLE (table5), radiobutton51, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton51), radiobutton51_group);
  radiobutton51_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton51));

  radiobutton52 = gtk_radio_button_new_with_mnemonic (NULL, _("High"));
  gtk_widget_show (radiobutton52);
  gtk_table_attach (GTK_TABLE (table5), radiobutton52, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton52), radiobutton51_group);
  radiobutton51_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton52));

  radiobutton53 = gtk_radio_button_new_with_mnemonic (NULL, _("Normal"));
  gtk_widget_show (radiobutton53);
  gtk_table_attach (GTK_TABLE (table5), radiobutton53, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton53), radiobutton53_group);
  radiobutton53_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton53));

  radiobutton54 = gtk_radio_button_new_with_mnemonic (NULL, _("Low"));
  gtk_widget_show (radiobutton54);
  gtk_table_attach (GTK_TABLE (table5), radiobutton54, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton54), radiobutton53_group);
  radiobutton53_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton54));

  frame43 = gtk_frame_new (NULL);
  gtk_widget_show (frame43);
  gtk_box_pack_start (GTK_BOX (hbox105), frame43, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame43), 10);
  gtk_widget_set_sensitive (frame43, FALSE);

  vbox66 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox66);
  gtk_container_add (GTK_CONTAINER (frame43), vbox66);
  gtk_container_set_border_width (GTK_CONTAINER (vbox66), 10);

  label278 = gtk_label_new (_("DSCP value (0-63)"));
  gtk_widget_show (label278);
  gtk_box_pack_start (GTK_BOX (vbox66), label278, FALSE, FALSE, 20);
  gtk_label_set_justify (GTK_LABEL (label278), GTK_JUSTIFY_CENTER);

  hbox111 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox111);
  gtk_box_pack_start (GTK_BOX (vbox66), hbox111, FALSE, FALSE, 0);

  entry154 = gtk_entry_new ();
  gtk_widget_show (entry154);
  gtk_box_pack_start (GTK_BOX (hbox111), entry154, TRUE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entry154), 2);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry154), 8226);

  dialog_action_area6 = GTK_DIALOG (tos_dialod)->action_area;
  gtk_widget_show (dialog_action_area6);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area6), GTK_BUTTONBOX_END);

  hbox103 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox103);
  gtk_container_add (GTK_CONTAINER (dialog_action_area6), hbox103);

  button76 = gtk_button_new_with_mnemonic (_("OK"));
  gtk_widget_show (button76);
  gtk_box_pack_start (GTK_BOX (hbox103), button76, TRUE, FALSE, 0);

  button77 = gtk_button_new_with_mnemonic (_("Cancel"));
  gtk_widget_show (button77);
  gtk_box_pack_start (GTK_BOX (hbox103), button77, TRUE, FALSE, 0);

  g_signal_connect ((gpointer) tos_dialod, "destroy",
                    G_CALLBACK (on_tos_dialod_destroy),
                    NULL);
  g_signal_connect ((gpointer) radiobutton38, "toggled",
                    G_CALLBACK (on_radiobutton38_toggled),
                    NULL);
  g_signal_connect ((gpointer) radiobutton39, "toggled",
                    G_CALLBACK (on_radiobutton39_toggled),
                    NULL);
  g_signal_connect ((gpointer) button76, "clicked",
                    G_CALLBACK (on_button76_clicked),
                    NULL);
  g_signal_connect ((gpointer) button77, "clicked",
                    G_CALLBACK (on_button77_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (tos_dialod, tos_dialod, "tos_dialod");
  GLADE_HOOKUP_OBJECT_NO_REF (tos_dialod, dialog_vbox6, "dialog_vbox6");
  GLADE_HOOKUP_OBJECT (tos_dialod, vbox64, "vbox64");
  GLADE_HOOKUP_OBJECT (tos_dialod, hbox104, "hbox104");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton38, "radiobutton38");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton39, "radiobutton39");
  GLADE_HOOKUP_OBJECT (tos_dialod, hbox105, "hbox105");
  GLADE_HOOKUP_OBJECT (tos_dialod, frame42, "frame42");
  GLADE_HOOKUP_OBJECT (tos_dialod, vbox65, "vbox65");
  GLADE_HOOKUP_OBJECT (tos_dialod, hbox106, "hbox106");
  GLADE_HOOKUP_OBJECT (tos_dialod, label277, "label277");
  GLADE_HOOKUP_OBJECT (tos_dialod, optionmenu13, "optionmenu13");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget132, "convertwidget132");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget133, "convertwidget133");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget134, "convertwidget134");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget135, "convertwidget135");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget136, "convertwidget136");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget137, "convertwidget137");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget138, "convertwidget138");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget139, "convertwidget139");
  GLADE_HOOKUP_OBJECT (tos_dialod, convertwidget140, "convertwidget140");
  GLADE_HOOKUP_OBJECT (tos_dialod, table5, "table5");
  GLADE_HOOKUP_OBJECT (tos_dialod, label279, "label279");
  GLADE_HOOKUP_OBJECT (tos_dialod, label280, "label280");
  GLADE_HOOKUP_OBJECT (tos_dialod, label281, "label281");
  GLADE_HOOKUP_OBJECT (tos_dialod, label282, "label282");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton47, "radiobutton47");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton48, "radiobutton48");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton49, "radiobutton49");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton50, "radiobutton50");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton51, "radiobutton51");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton52, "radiobutton52");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton53, "radiobutton53");
  GLADE_HOOKUP_OBJECT (tos_dialod, radiobutton54, "radiobutton54");
  GLADE_HOOKUP_OBJECT (tos_dialod, frame43, "frame43");
  GLADE_HOOKUP_OBJECT (tos_dialod, vbox66, "vbox66");
  GLADE_HOOKUP_OBJECT (tos_dialod, label278, "label278");
  GLADE_HOOKUP_OBJECT (tos_dialod, hbox111, "hbox111");
  GLADE_HOOKUP_OBJECT (tos_dialod, entry154, "entry154");
  GLADE_HOOKUP_OBJECT_NO_REF (tos_dialod, dialog_action_area6, "dialog_action_area6");
  GLADE_HOOKUP_OBJECT (tos_dialod, hbox103, "hbox103");
  GLADE_HOOKUP_OBJECT (tos_dialod, button76, "button76");
  GLADE_HOOKUP_OBJECT (tos_dialod, button77, "button77");

  return tos_dialod;
}