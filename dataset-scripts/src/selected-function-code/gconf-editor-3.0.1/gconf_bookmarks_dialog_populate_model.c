static void
gconf_bookmarks_dialog_populate_model (GConfBookmarksDialog *dialog)
{
	GConfClient *client;
	GSList *value_list, *p;
	GtkTreeIter iter;
	
	client = gconf_client_get_default ();
	
	/* First clear the list store */
	dialog->changing_model = TRUE;
	gtk_list_store_clear (dialog->list_store);

	value_list = gconf_client_get_list (client, BOOKMARKS_KEY,
					    GCONF_VALUE_STRING, NULL);

	for (p = value_list; p; p = p->next) {
		gtk_list_store_append (dialog->list_store, &iter);
		gtk_list_store_set (dialog->list_store, &iter,
				    0, p->data,
				    -1);
		g_free (p->data);
	}

	if (value_list)
		g_slist_free (value_list);

	dialog->changing_model = FALSE;

	g_object_unref (client);
}