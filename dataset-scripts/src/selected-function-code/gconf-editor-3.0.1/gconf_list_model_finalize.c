static void
gconf_list_model_finalize (GObject *object)
{
	GConfListModel *list_model;

	list_model = (GConfListModel *)object;
	
	g_hash_table_destroy (list_model->key_hash);

	if (list_model->client && list_model->notify_id > 0) {
		gconf_client_notify_remove (list_model->client, list_model->notify_id);
	}

	if (list_model->client && list_model->root_path) {
		gconf_client_remove_dir  (list_model->client,
					  list_model->root_path, NULL);
	}

        if (list_model->client) {
                g_object_unref (list_model->client);
		list_model->client = NULL;
        }

	if (list_model->values) {
		g_slist_foreach (list_model->values, (GFunc) gconf_entry_unref, NULL);
		g_slist_free (list_model->values);
		list_model->values = NULL;
	}

	g_free (list_model->root_path);

	G_OBJECT_CLASS (gconf_list_model_parent_class)->finalize (object);
}