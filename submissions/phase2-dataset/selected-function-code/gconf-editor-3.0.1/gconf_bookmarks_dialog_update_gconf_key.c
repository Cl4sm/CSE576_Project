static void
gconf_bookmarks_dialog_update_gconf_key (GConfBookmarksDialog *dialog)
{
	GSList *list;
	GtkTreeIter iter;
	char *bookmark;
	GConfClient *client;
	
	list = NULL;

	if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (dialog->list_store), &iter)) {
		do {
			gtk_tree_model_get (GTK_TREE_MODEL (dialog->list_store), &iter,
					    0, &bookmark,
					    -1);
			list = g_slist_append (list, bookmark);
		} while (gtk_tree_model_iter_next (GTK_TREE_MODEL (dialog->list_store), &iter));
	}

	client = gconf_client_get_default ();

	dialog->changing_key = TRUE;
	gconf_client_set_list (client, BOOKMARKS_KEY,
			       GCONF_VALUE_STRING, list, NULL);

	g_object_unref (client);
}