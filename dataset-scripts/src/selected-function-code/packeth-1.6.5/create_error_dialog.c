GtkWidget*
create_error_dialog (void)
{
  GtkWidget *error_dialog;
  GtkWidget *dialog_vbox3;
  GtkWidget *hbox67;
  GtkWidget *pixmap1;
  GtkWidget *label164;
  GtkWidget *label165;
  GtkWidget *dialog_action_area3;
  GtkWidget *button52;

  error_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (error_dialog), _("PackETH: Error"));
  gtk_window_set_position (GTK_WINDOW (error_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_type_hint (GTK_WINDOW (error_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox3 = GTK_DIALOG (error_dialog)->vbox;
  gtk_widget_show (dialog_vbox3);

  hbox67 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox67);
  gtk_box_pack_start (GTK_BOX (dialog_vbox3), hbox67, TRUE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox67), 20);

  pixmap1 = create_pixmap (error_dialog, "X.xpm");
  gtk_widget_show (pixmap1);
  gtk_box_pack_start (GTK_BOX (hbox67), pixmap1, FALSE, FALSE, 20);

  label164 = gtk_label_new (_("  "));
  gtk_widget_show (label164);
  gtk_box_pack_start (GTK_BOX (hbox67), label164, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label164), GTK_JUSTIFY_CENTER);

  label165 = gtk_label_new (_("label165"));
  gtk_widget_show (label165);
  gtk_box_pack_start (GTK_BOX (hbox67), label165, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label165), GTK_JUSTIFY_CENTER);

  dialog_action_area3 = GTK_DIALOG (error_dialog)->action_area;
  gtk_widget_show (dialog_action_area3);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area3), GTK_BUTTONBOX_END);

  button52 = gtk_button_new_with_mnemonic (_("OK"));
  gtk_widget_show (button52);
  gtk_dialog_add_action_widget (GTK_DIALOG (error_dialog), button52, 0);

  g_signal_connect ((gpointer) error_dialog, "destroy",
                    G_CALLBACK (on_error_dialog_destroy),
                    NULL);
  g_signal_connect ((gpointer) button52, "clicked",
                    G_CALLBACK (on_button52_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (error_dialog, error_dialog, "error_dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (error_dialog, dialog_vbox3, "dialog_vbox3");
  GLADE_HOOKUP_OBJECT (error_dialog, hbox67, "hbox67");
  GLADE_HOOKUP_OBJECT (error_dialog, pixmap1, "pixmap1");
  GLADE_HOOKUP_OBJECT (error_dialog, label164, "label164");
  GLADE_HOOKUP_OBJECT (error_dialog, label165, "label165");
  GLADE_HOOKUP_OBJECT_NO_REF (error_dialog, dialog_action_area3, "dialog_action_area3");
  GLADE_HOOKUP_OBJECT (error_dialog, button52, "button52");

  return error_dialog;
}