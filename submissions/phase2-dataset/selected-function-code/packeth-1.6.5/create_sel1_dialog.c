GtkWidget*
create_sel1_dialog (void)
{
  GtkWidget *sel1_dialog;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox14;
  GtkWidget *hbox17;
  GtkWidget *scrolledwindow3;
  GtkWidget *clist1;
  GtkWidget *hbox18;
  GtkWidget *label44;
  GtkWidget *sel1_IP_entry;
  GtkWidget *label45;
  GtkWidget *sel1_mac_entry;
  GtkWidget *label274;
  GtkWidget *entry153;
  GtkWidget *dialog_action_area1;
  GtkWidget *hbox19;
  GtkWidget *sel1_add_bt;
  GtkWidget *sel1_delete_bt;
  GtkWidget *sel1_ok_bt;
  GtkWidget *sel1_cancel_bt;

  sel1_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (sel1_dialog), _("Address database"));
  gtk_window_set_default_size (GTK_WINDOW (sel1_dialog), -1, 250);
  gtk_window_set_type_hint (GTK_WINDOW (sel1_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (sel1_dialog)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox14 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox14);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox14, TRUE, TRUE, 0);

  hbox17 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox17);
  gtk_box_pack_start (GTK_BOX (vbox14), hbox17, TRUE, TRUE, 0);

  scrolledwindow3 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow3);
  gtk_box_pack_start (GTK_BOX (hbox17), scrolledwindow3, TRUE, TRUE, 0);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow3, GTK_CAN_FOCUS);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow3), GTK_SHADOW_IN);

  clist1 = gtk_tree_view_new ();
  gtk_widget_show (clist1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow3), clist1);
  gtk_container_set_border_width (GTK_CONTAINER (clist1), 10);

  hbox18 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox18);
  gtk_box_pack_start (GTK_BOX (vbox14), hbox18, FALSE, TRUE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (hbox18), 5);

  label44 = gtk_label_new (_("IP"));
  gtk_widget_show (label44);
  gtk_box_pack_start (GTK_BOX (hbox18), label44, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label44), GTK_JUSTIFY_CENTER);

  sel1_IP_entry = gtk_entry_new ();
  gtk_widget_show (sel1_IP_entry);
  gtk_box_pack_start (GTK_BOX (hbox18), sel1_IP_entry, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (sel1_IP_entry), 15);
  gtk_entry_set_invisible_char (GTK_ENTRY (sel1_IP_entry), 8226);

  label45 = gtk_label_new (_("     MAC"));
  gtk_widget_show (label45);
  gtk_box_pack_start (GTK_BOX (hbox18), label45, FALSE, FALSE, 5);
  gtk_label_set_justify (GTK_LABEL (label45), GTK_JUSTIFY_CENTER);

  sel1_mac_entry = gtk_entry_new ();
  gtk_widget_show (sel1_mac_entry);
  gtk_box_pack_start (GTK_BOX (hbox18), sel1_mac_entry, FALSE, FALSE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (sel1_mac_entry), 17);
  gtk_entry_set_invisible_char (GTK_ENTRY (sel1_mac_entry), 8226);

  label274 = gtk_label_new (_("     Name"));
  gtk_widget_show (label274);
  gtk_box_pack_start (GTK_BOX (hbox18), label274, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label274), GTK_JUSTIFY_CENTER);

  entry153 = gtk_entry_new ();
  gtk_widget_show (entry153);
  gtk_box_pack_start (GTK_BOX (hbox18), entry153, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (entry153), 50);
  gtk_entry_set_invisible_char (GTK_ENTRY (entry153), 8226);

  dialog_action_area1 = GTK_DIALOG (sel1_dialog)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  hbox19 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox19);
  gtk_container_add (GTK_CONTAINER (dialog_action_area1), hbox19);

  sel1_add_bt = gtk_button_new_with_mnemonic (_("Add"));
  gtk_widget_show (sel1_add_bt);
  gtk_box_pack_start (GTK_BOX (hbox19), sel1_add_bt, TRUE, FALSE, 0);

  sel1_delete_bt = gtk_button_new_with_mnemonic (_("Delete"));
  gtk_widget_show (sel1_delete_bt);
  gtk_box_pack_start (GTK_BOX (hbox19), sel1_delete_bt, TRUE, FALSE, 0);

  sel1_ok_bt = gtk_button_new_with_mnemonic (_("Ok"));
  gtk_widget_show (sel1_ok_bt);
  gtk_box_pack_start (GTK_BOX (hbox19), sel1_ok_bt, TRUE, FALSE, 0);

  sel1_cancel_bt = gtk_button_new_with_mnemonic (_("Cancel"));
  gtk_widget_show (sel1_cancel_bt);
  gtk_box_pack_start (GTK_BOX (hbox19), sel1_cancel_bt, TRUE, FALSE, 0);

  g_signal_connect ((gpointer) sel1_dialog, "destroy",
                    G_CALLBACK (on_sel1_dialog_destroy),
                    NULL);
  g_signal_connect ((gpointer) sel1_add_bt, "clicked",
                    G_CALLBACK (on_sel1_add_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) sel1_delete_bt, "clicked",
                    G_CALLBACK (on_sel1_delete_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) sel1_ok_bt, "clicked",
                    G_CALLBACK (on_sel1_ok_bt_clicked),
                    NULL);
  g_signal_connect ((gpointer) sel1_cancel_bt, "clicked",
                    G_CALLBACK (on_sel1_cancel_bt_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (sel1_dialog, sel1_dialog, "sel1_dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (sel1_dialog, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (sel1_dialog, vbox14, "vbox14");
  GLADE_HOOKUP_OBJECT (sel1_dialog, hbox17, "hbox17");
  GLADE_HOOKUP_OBJECT (sel1_dialog, scrolledwindow3, "scrolledwindow3");
  GLADE_HOOKUP_OBJECT (sel1_dialog, clist1, "clist1");
  GLADE_HOOKUP_OBJECT (sel1_dialog, hbox18, "hbox18");
  GLADE_HOOKUP_OBJECT (sel1_dialog, label44, "label44");
  GLADE_HOOKUP_OBJECT (sel1_dialog, sel1_IP_entry, "sel1_IP_entry");
  GLADE_HOOKUP_OBJECT (sel1_dialog, label45, "label45");
  GLADE_HOOKUP_OBJECT (sel1_dialog, sel1_mac_entry, "sel1_mac_entry");
  GLADE_HOOKUP_OBJECT (sel1_dialog, label274, "label274");
  GLADE_HOOKUP_OBJECT (sel1_dialog, entry153, "entry153");
  GLADE_HOOKUP_OBJECT_NO_REF (sel1_dialog, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (sel1_dialog, hbox19, "hbox19");
  GLADE_HOOKUP_OBJECT (sel1_dialog, sel1_add_bt, "sel1_add_bt");
  GLADE_HOOKUP_OBJECT (sel1_dialog, sel1_delete_bt, "sel1_delete_bt");
  GLADE_HOOKUP_OBJECT (sel1_dialog, sel1_ok_bt, "sel1_ok_bt");
  GLADE_HOOKUP_OBJECT (sel1_dialog, sel1_cancel_bt, "sel1_cancel_bt");

  return sel1_dialog;
}