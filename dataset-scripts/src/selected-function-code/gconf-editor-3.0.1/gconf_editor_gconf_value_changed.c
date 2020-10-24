static void
gconf_editor_gconf_value_changed (GConfCellRenderer *cell, const gchar *path_str, GConfValue *new_value, GConfEditorWindow *window)
{
	GtkTreeIter iter;
	GtkTreePath *path;
	gchar *key;
	GConfValue *value;
	
	path = gtk_tree_path_new_from_string (path_str);

	gtk_tree_model_get_iter (window->sorted_list_model, &iter, path);

	gtk_tree_model_get (window->sorted_list_model, &iter,
			    GCONF_LIST_MODEL_KEY_PATH_COLUMN, &key,
			    GCONF_LIST_MODEL_VALUE_COLUMN, &value,
			    -1);

	/* We need to check this because the changed signal could come from an old
	 * cell in another list_model */ 
	if (value->type == new_value->type) {
		gconf_client_set (window->client, key, new_value, NULL);
		gconf_add_recent_key (window, key);
	}

	gconf_value_free (value);
	g_free (key);
	gtk_tree_path_free (path);
}