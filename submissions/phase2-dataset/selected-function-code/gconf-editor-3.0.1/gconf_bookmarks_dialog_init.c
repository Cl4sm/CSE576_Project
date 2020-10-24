static void
gconf_bookmarks_dialog_init (GConfBookmarksDialog *dialog)
{
	GtkWidget *scrolled_window, *hbox, *vbox;
	GtkWidget *content_area;
	GtkCellRenderer *cell;
	GtkTreeViewColumn *column;
	GConfClient *client;
       
	hbox = gtk_hbox_new (FALSE, 6);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);

	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

	gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
	gtk_box_set_spacing (GTK_BOX (content_area), 2);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);
	gtk_window_set_default_size (GTK_WINDOW (dialog), 300, 200);
	
	gtk_dialog_add_button (GTK_DIALOG (dialog), GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE);
	gtk_window_set_title (GTK_WINDOW (dialog), _("Edit Bookmarks"));

	dialog->list_store = gtk_list_store_new (1, G_TYPE_STRING);
	g_signal_connect (dialog->list_store, "row_deleted",
			  G_CALLBACK (gconf_bookmarks_dialog_row_deleted), dialog);
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start (GTK_BOX (hbox), scrolled_window, TRUE, TRUE, 0);
	
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window), GTK_SHADOW_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	dialog->tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (dialog->list_store));
	gtk_tree_view_set_reorderable (GTK_TREE_VIEW (dialog->tree_view), TRUE);
	
	g_signal_connect (gtk_tree_view_get_selection (GTK_TREE_VIEW (dialog->tree_view)), "changed",
			  G_CALLBACK (gconf_bookmarks_dialog_selection_changed), dialog);
	gconf_bookmarks_dialog_populate_model (dialog);

	column = gtk_tree_view_column_new ();

	cell = gtk_cell_renderer_pixbuf_new ();
	g_object_set (G_OBJECT (cell),
		      "stock-id", STOCK_BOOKMARK,
		      NULL);
	gtk_tree_view_column_pack_start (column, cell, FALSE);
	
	cell = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, cell, TRUE);
	gtk_tree_view_column_set_attributes (column, cell,
					     "text", 0,
					     NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (dialog->tree_view), column);

	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (dialog->tree_view), FALSE);
	
	g_object_unref (dialog->list_store);
	gtk_container_add (GTK_CONTAINER (scrolled_window), dialog->tree_view);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, FALSE, 0);
	dialog->delete_button = gtk_button_new_from_stock (GTK_STOCK_DELETE);
	gtk_widget_set_sensitive (dialog->delete_button, FALSE);
	g_signal_connect (dialog->delete_button, "clicked",
			  G_CALLBACK (gconf_bookmarks_dialog_delete_bookmark), dialog);
	
	gtk_box_pack_start (GTK_BOX (vbox), dialog->delete_button, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (content_area), hbox, TRUE, TRUE, 0);
	gtk_widget_show_all (hbox);

	/* Listen for gconf changes */
	client = gconf_client_get_default ();
	gconf_client_add_dir (client, BOOKMARKS_KEY, GCONF_CLIENT_PRELOAD_NONE, NULL);

	dialog->notify_id = gconf_client_notify_add (client, BOOKMARKS_KEY,
						     gconf_bookmarks_dialog_bookmarks_key_changed,
						     dialog,
						     NULL,
						     NULL);

	g_object_unref (client);
}