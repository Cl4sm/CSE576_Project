static void
gconf_add_recent_key (GConfEditorWindow *window, const char *key)
{
	GSList *list, *tmp;
	gboolean add = TRUE;

	/* Get the old list and then set it */
	list = gconf_client_get_list (window->client,
				      "/apps/gconf-editor/recents", GCONF_VALUE_STRING, NULL);

	/* Check that the recent hasn't been added already */
	for (tmp = list; tmp; tmp = tmp->next) {
		if (strcmp ((gchar*) tmp->data, key) == 0)
			add = FALSE;
	}

	/* Append the new key */
	if (add == TRUE) {
		/* Keep list on a fixed size, remove the last */
		if (g_slist_length (list) > RECENT_LIST_MAX_SIZE - 1) {
			tmp = g_slist_last (list);
			g_free (tmp->data);
			list = g_slist_delete_link (list, tmp);
		}

		list = g_slist_prepend (list, g_strdup (key));

		gconf_client_set_list (window->client,
				       "/apps/gconf-editor/recents", GCONF_VALUE_STRING, list, NULL);

		if (gtk_widget_get_visible (GTK_WIDGET (window->output_window)) &&
	    				window->output_window_type == GCONF_EDITOR_WINDOW_OUTPUT_WINDOW_RECENTS) 
			gedit_output_window_prepend_line (GEDIT_OUTPUT_WINDOW (window->output_window),
							  (gchar*) key, TRUE);
	}


	for (tmp = list; tmp; tmp = tmp->next) {
		g_free (tmp->data);
	}

	g_slist_free (list);
}