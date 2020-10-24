static gboolean
gconf_editor_check_writable (GConfCellRenderer *cell, const gchar *path_str, GConfEditorWindow *window)
{
	GtkTreeIter iter;
	GtkTreePath *path;
	gchar *key;
	gboolean ret;
	
	path = gtk_tree_path_new_from_string (path_str);

	gtk_tree_model_get_iter (window->sorted_list_model, &iter, path);

	gtk_tree_model_get (window->sorted_list_model, &iter,
			    GCONF_LIST_MODEL_KEY_PATH_COLUMN, &key,
			    -1);

	ret = gconf_client_key_is_writable (window->client, key, NULL);

	g_free (key);

	return ret;
}