static
gboolean
gconf_tree_model_search_iter_foreach (GtkTreeModel *model, GtkTreePath *path,
				      GtkTreeIter *iter, gpointer data)
{
	Node    *node;
	SearchIter *st;
	gchar *found;
	GSList *values, *list;

	st = (SearchIter *) data;

	if (st->searching == NULL) {
		return TRUE;
	}

	if (st->res >= 1) {
		gtk_widget_show (GTK_WIDGET (st->output_window));
	}
	while (gtk_events_pending ())
		gtk_main_iteration ();

	node = iter->user_data;
	found = g_strrstr ((char*) node->path, (char*) st->pattern);

	if (found != NULL) {
		/* We found the pattern in the tree */
		gchar *key = gconf_tree_model_get_gconf_path (GCONF_TREE_MODEL (model), iter);
		gedit_output_window_append_line (st->output_window, key, FALSE);
		g_free (key);
		st->res++;
		return FALSE;
	}

	if (!st->search_keys && !st->search_values) {
		return FALSE;
	}
	values = gconf_client_all_entries (GCONF_TREE_MODEL (model)->client, (const char*) node->path , NULL);
	for (list = values; list; list = list->next) {
		const gchar *key;
		GConfEntry *entry = list->data;
		key = gconf_entry_get_key (entry);
		/* Search in the key names */
		if (st->search_keys) {
			found = g_strrstr (key, (char*) st->pattern);
			if (found != NULL) {
				/* We found the pattern in the final key name */
				gedit_output_window_append_line (st->output_window, key, FALSE);
				st->res++;
				gconf_entry_unref (entry);
				/* After finding an entry continue the list to find other matches */
				continue;
			}
		}

		/* Search in the values */
		if (st->search_values) {
			const char *gconf_string;
			GConfValue *gconf_value = gconf_entry_get_value (entry);

			/* FIXME: We are only looking into strings... should we do in
			 * int's? */
			if (gconf_value != NULL && gconf_value->type == GCONF_VALUE_STRING)
				gconf_string = gconf_value_get_string (gconf_value);
			else {
				gconf_entry_unref (entry);
				continue;
			}

                	found = g_strrstr (gconf_string, (char*) st->pattern);
			if (found != NULL) {
				/* We found the pattern in the key value */
				gedit_output_window_append_line (st->output_window, key, FALSE);
				st->res++;
				gconf_entry_unref (entry);
				continue;
			}
		}
		gconf_entry_unref (entry);
	}

	return FALSE;
}