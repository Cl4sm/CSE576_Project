static void
gconf_search_dialog_init (GConfSearchDialog *dialog)
{
	GtkWidget *action_area, *content_area;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *label;

	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	action_area = gtk_dialog_get_action_area (GTK_DIALOG (dialog));

	gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
	gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
	gtk_box_set_spacing (GTK_BOX (content_area), 2);
	
	hbox = gtk_hbox_new (FALSE, 12);
	vbox = gtk_vbox_new (FALSE, 6);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);

	gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE);

	gtk_window_set_title (GTK_WINDOW (dialog), _("Find"));

	label = gtk_label_new_with_mnemonic (_("_Search for: "));
			
	dialog->entry = gtk_entry_new ();
	g_signal_connect (dialog->entry, "changed",
			  G_CALLBACK (gconf_search_entry_changed), dialog);
	gtk_label_set_mnemonic_widget (GTK_LABEL(label), GTK_WIDGET(dialog->entry));
	
	dialog->search_button = gtk_button_new_from_stock (GTK_STOCK_FIND);
	gtk_widget_set_sensitive (dialog->search_button, FALSE);
	g_signal_connect (dialog->search_button, "clicked",
			  G_CALLBACK (gconf_search_dialog_search), dialog);

	gtk_widget_set_can_default (dialog->search_button, TRUE);
	gtk_window_set_default(GTK_WINDOW(dialog), dialog->search_button);
	gtk_entry_set_activates_default(GTK_ENTRY(dialog->entry), TRUE);
	
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), dialog->entry, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

	dialog->search_in_keys = gtk_check_button_new_with_mnemonic (_("Search also in key _names"));
	gtk_box_pack_start (GTK_BOX (vbox), dialog->search_in_keys, TRUE, TRUE, 0);

	dialog->search_in_values = gtk_check_button_new_with_mnemonic (_("Search also in key _values"));
	gtk_box_pack_start (GTK_BOX (vbox), dialog->search_in_values, TRUE, TRUE, 0);

	gtk_box_pack_start (GTK_BOX (content_area), vbox, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (action_area), dialog->search_button, TRUE, TRUE, 0);
	gtk_widget_show_all (action_area);
	gtk_widget_show_all (vbox);
	
	gtk_widget_show_all (hbox);
}