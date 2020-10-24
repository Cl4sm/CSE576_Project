GtkWidget*
create_about_dialog (void)
{
  GtkWidget *about_dialog;
  GtkWidget *dialog_vbox5;
  GtkWidget *vbox63;
  GtkWidget *pixmap17;
  GtkWidget *label275;
  GtkWidget *label276;
  GtkWidget *dialog_action_area5;
  GtkWidget *button75;

  about_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (about_dialog), _("About"));
  gtk_window_set_type_hint (GTK_WINDOW (about_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox5 = GTK_DIALOG (about_dialog)->vbox;
  gtk_widget_show (dialog_vbox5);

  vbox63 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox63);
  gtk_box_pack_start (GTK_BOX (dialog_vbox5), vbox63, TRUE, TRUE, 0);

  pixmap17 = create_pixmap (about_dialog, NULL);
  gtk_widget_show (pixmap17);
  gtk_box_pack_start (GTK_BOX (vbox63), pixmap17, FALSE, FALSE, 15);

  label275 = gtk_label_new (_("packETH - 1.6"));
  gtk_widget_show (label275);
  gtk_box_pack_start (GTK_BOX (vbox63), label275, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label275), GTK_JUSTIFY_CENTER);

  label276 = gtk_label_new (_("ethernet packet generator\n          Copyright (C) 2003 - Miha Jemec, Iskratel          \n\nhttp://packeth.sourceforge.net/"));
  gtk_widget_show (label276);
  gtk_box_pack_start (GTK_BOX (vbox63), label276, FALSE, FALSE, 10);
  gtk_label_set_justify (GTK_LABEL (label276), GTK_JUSTIFY_CENTER);

  dialog_action_area5 = GTK_DIALOG (about_dialog)->action_area;
  gtk_widget_show (dialog_action_area5);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area5), GTK_BUTTONBOX_END);

  button75 = gtk_button_new_with_mnemonic (_("Close"));
  gtk_widget_show (button75);
  gtk_dialog_add_action_widget (GTK_DIALOG (about_dialog), button75, 0);

  g_signal_connect ((gpointer) about_dialog, "destroy",
                    G_CALLBACK (on_about_dialog_destroy),
                    NULL);
  g_signal_connect ((gpointer) button75, "clicked",
                    G_CALLBACK (on_button75_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (about_dialog, about_dialog, "about_dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (about_dialog, dialog_vbox5, "dialog_vbox5");
  GLADE_HOOKUP_OBJECT (about_dialog, vbox63, "vbox63");
  GLADE_HOOKUP_OBJECT (about_dialog, pixmap17, "pixmap17");
  GLADE_HOOKUP_OBJECT (about_dialog, label275, "label275");
  GLADE_HOOKUP_OBJECT (about_dialog, label276, "label276");
  GLADE_HOOKUP_OBJECT_NO_REF (about_dialog, dialog_action_area5, "dialog_action_area5");
  GLADE_HOOKUP_OBJECT (about_dialog, button75, "button75");

  return about_dialog;
}