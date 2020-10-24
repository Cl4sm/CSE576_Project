void
gconf_list_model_set_root_path (GConfListModel *model, const gchar *root_path)
{
	GSList *list;
	GSList *values;
	GtkTreeIter iter;
	GtkTreePath *path;

	path = gtk_tree_path_new ();
	gtk_tree_path_append_index (path, 0);

	if (model->root_path != NULL) {
		for (list = model->values; list; list = list->next) {
			model->stamp++;
			gtk_tree_model_row_deleted (GTK_TREE_MODEL (model), path);
		}

		for (list = model->values; list; list = list->next) {
			GConfEntry *entry = list->data;

			g_hash_table_remove (model->key_hash, gconf_entry_get_key (entry));

			gconf_entry_unref (entry);
		}

		gconf_client_notify_remove (model->client, model->notify_id);

		gconf_client_remove_dir  (model->client,
					  model->root_path, NULL);

		g_free (model->root_path);
		g_slist_free (model->values);
		model->values = NULL;
	}
	gtk_tree_path_free (path);

	gconf_client_add_dir (model->client,
			      root_path,
			      GCONF_CLIENT_PRELOAD_ONELEVEL,
			      NULL);

	model->notify_id = gconf_client_notify_add (model->client, root_path,
						    gconf_list_model_notify_func,
						    model, NULL, NULL);

	model->root_path = g_strdup (root_path);
	values = gconf_client_all_entries (model->client, root_path, NULL);
	model->length = 0;

	for (list = values; list; list = list->next) {
		GConfEntry *entry = list->data;

		model->values = g_slist_append (model->values, list->data);
		model->length++;

		model->stamp++;

		iter.stamp = model->stamp;
		iter.user_data = g_slist_last (model->values);

		g_hash_table_insert (model->key_hash, g_strdup (gconf_entry_get_key (entry)), iter.user_data);

		path = gtk_tree_model_get_path (GTK_TREE_MODEL (model), &iter);
		gtk_tree_model_row_inserted (GTK_TREE_MODEL (model), path, &iter);

		gtk_tree_path_free (path);
	}

	if (values) 
		g_slist_free (values);
}