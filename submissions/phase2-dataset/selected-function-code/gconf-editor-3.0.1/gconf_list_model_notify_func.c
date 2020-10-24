static void
gconf_list_model_notify_func (GConfClient* client, guint cnxn_id, GConfEntry *entry, gpointer user_data)
{
	GSList *list;
	const gchar *key;
	char *path_str;
	GConfListModel *list_model = user_data;
	GtkTreeIter iter;
	GtkTreePath *path;

	key = gconf_entry_get_key (entry);

	path_str = g_path_get_dirname (key);

	if (strcmp (path_str, list_model->root_path) != 0)
	  {
	    g_free (path_str);
	    return;
	  }

	g_free (path_str);

	if (strncmp (key, list_model->root_path, strlen (list_model->root_path)) != 0)
	    return;
	
	if (gconf_client_dir_exists (client, key, NULL))
		/* this is a directory -- ignore */
		return;

	list = g_hash_table_lookup (list_model->key_hash, key);

	if (list == NULL) {
		/* Create a new entry */
		entry = gconf_entry_new (gconf_entry_get_key (entry),
					 gconf_entry_get_value (entry));

		list = g_slist_append (list, entry);
		list_model->values = g_slist_concat (list_model->values, list);
		g_hash_table_insert (list_model->key_hash, g_strdup (key), list);

		list_model->stamp++;

		iter.stamp = list_model->stamp;
		iter.user_data = list;

		list_model->length++;

		path = gtk_tree_model_get_path (GTK_TREE_MODEL (list_model), &iter);
		gtk_tree_model_row_inserted (GTK_TREE_MODEL (list_model), path, &iter);
		gtk_tree_path_free (path);
	}
	else {
		list_model->stamp++;

		iter.stamp = list_model->stamp;
		iter.user_data = list;

		path = gtk_tree_model_get_path (GTK_TREE_MODEL (list_model), &iter);

		gconf_entry_unref (list->data);

		if (gconf_entry_get_value (entry) != NULL) {
			list->data = gconf_entry_new (gconf_entry_get_key (entry),
						      gconf_entry_get_value (entry));
			gtk_tree_model_row_changed (GTK_TREE_MODEL (list_model), path, &iter);
		}
		else {
			gtk_tree_model_row_deleted (GTK_TREE_MODEL (list_model), path);
			list_model->values = g_slist_remove (list_model->values, list->data);
			list_model->length--;
			g_hash_table_remove (list_model->key_hash, key);
		}

		gtk_tree_path_free (path);
	}
}